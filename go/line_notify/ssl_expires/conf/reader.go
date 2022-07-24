package conf

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strconv"

	"gopkg.in/ini.v1"
)

type Config struct {
	Domains         []string
	NotifiedExpires []int
}

func getFilePath() string {
	exePath, _ := os.Executable()
	fileName := "ssl_expires.ini"

	// check execute directory
	cfgPath := filepath.Join(filepath.Dir(exePath), fileName)
	if _, err := os.Stat(cfgPath); errors.Is(err, os.ErrNotExist) {
		// check current directory
		if _, err := os.Stat(fileName); errors.Is(err, os.ErrNotExist) {
			// check parent directory
			fileName = filepath.Join("..", fileName)
			if _, err := os.Stat(fileName); errors.Is(err, os.ErrNotExist) {
				fmt.Printf("Cannot find config file %v %v\n", cfgPath, fileName)
				os.Exit(1)
			}
		}
		cfgPath = fileName
	}

	return cfgPath
}

func Read() Config {
	cfgPath := getFilePath()
	cfg, _ := ini.Load(cfgPath)

	var domains []string
	var expires []int

	for _, fqdn := range cfg.Section("domain").KeyStrings() {
		if cfg.Section("domain").Key(fqdn).MustBool(false) {
			domains = append(domains, fqdn)
		}
	}

	for _, days := range cfg.Section("expires").KeyStrings() {
		if cfg.Section("expires").Key(days).MustBool(false) {
			val, _ := strconv.Atoi(days)
			expires = append(expires, val)
		}
	}

	return Config{
		Domains:         domains,
		NotifiedExpires: expires,
	}
}
