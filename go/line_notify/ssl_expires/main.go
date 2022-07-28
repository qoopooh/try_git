package main

import (
	"fmt"
	"line_notify/ssl_expires/conf"
	"line_notify/ssl_expires/recorder"
	"line_notify/ssl_expires/ssl"
	"os"
	"path/filepath"
	"sort"
	"time"

	"github.com/akamensky/argparse"
	"github.com/qoopooh/linenotify/notify"
)

type Number interface {
	int | int32 | int64
}

func MaxNumberSlice[V Number](v []V) V {
	max := v[0]
	for i := 1; i < len(v); i++ {
		if v[i] > max {
			max = v[i]
		}
	}
	return max
}

func query(cfg conf.Config) []ssl.Result {

	c := make(chan ssl.Result, 1)

	for _, fqdn := range cfg.Domains {
		go ssl.Query(fqdn, c)
	}

	out := []ssl.Result{}
	for i := 0; i < len(cfg.Domains); i++ {
		out = append(out, <-c)
	}

	// Longest expiry first
	sort.SliceStable(out, func(i, j int) bool {
		return out[i].Expiry.After(out[j].Expiry)
	})

	return out
}

func isNotified(num int, arr []int) bool {
	for _, val := range arr {
		if val == num {
			return true
		}
	}

	return false
}

func shouldNotify(domain ssl.Result, cfg conf.Config, recorder *recorder.Recorder) bool {

	days_to_notify := false

	for _, days := range cfg.NotifiedExpires {
		if domain.DaysToExpires == days {
			days_to_notify = true
			break
		}
	}

	if !days_to_notify {
		return false
	}

	return recorder.ShouldNotify(domain)
}

func createMessage(domain ssl.Result) string {
	return fmt.Sprintf("%s expires in %d day(s)",
		domain.Fqdn,
		domain.DaysToExpires)
}

func run(dry bool) {
	cfg := conf.Read()
	recorder := recorder.Recorder{}
	recorder.Load()

	if dry {
		fmt.Printf("Domains: %+v\n", cfg.Domains)
		fmt.Printf("NotifiedExpires: %+v\n", cfg.NotifiedExpires)
		fmt.Printf("Recorder: %+v\n\n", recorder)
	}

	out := query(cfg)

	for i := 0; i < len(out); i++ {
		domain := out[i]
		if dry {
			fmt.Printf("%s (%d): %s\n",
				domain.Expiry.Format(time.RFC3339),
				domain.DaysToExpires,
				domain.Fqdn)
		}

		if shouldNotify(domain, cfg, &recorder) {
			msg := createMessage(domain)
			if dry {
				fmt.Println("notify", msg)
			} else {
				param := notify.SendOpts{
					Prefix:  getExeFilename(),
					Token:   os.Getenv("LINE_NOTIFY_TOKEN"),
					Message: msg,
					Verbose: true,
				}
				notify.Send(param)
			}
		}
	}

	recorder.Save()
	if dry {
		fmt.Printf("\nSaved: %+v\n\n", recorder)
	}
}

func getExeFilename() string {
	exePath, _ := os.Executable()
	return filepath.Base(exePath)
}

func main() {

	// Create new parser object
	parser := argparse.NewParser("ssl_expires", "https://github.com/akamensky/argparse")
	dry := parser.Flag("d", "dry", &argparse.Options{Help: "Dry-run"})
	// Parse input
	err := parser.Parse(os.Args)
	if err != nil {
		// In case of error print error and print usage
		// This can also be done by passing -h or --help flags
		fmt.Print(parser.Usage(err))
		os.Exit(0)
	}

	// Run the SSL expires
	run(*dry)
}
