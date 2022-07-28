package recorder

import (
	"fmt"
	"line_notify/ssl_expires/ssl"
	"sort"
	"strconv"
	"strings"

	"github.com/qoopooh/linenotify/tmp"
)

type Recorder struct {
	Count   int
	store   tmp.Store
	records map[string]string
}

func (r *Recorder) Load() {
	r.store = tmp.Store{
		FileName: "ssl_expires.json",
	}
	r.store.Get(&r.records)

	r.Count = 0
	for _ = range r.records {
		r.Count += 1
	}
}

func (r *Recorder) Save() {
	r.store.Set(r.records)
}

func (r *Recorder) Clear() {
	r.Count = 0
	r.records = make(map[string]string)
	r.Save()
}

// Check domain record exists. If not, just record it
func (r *Recorder) ShouldNotify(domain ssl.Result) bool {

	val, ok := r.records[domain.Fqdn]
	if !ok {
		r.records[domain.Fqdn] = fmt.Sprintf("%d", domain.DaysToExpires)
		r.Count += 1
		return true
	}

	arr := strToIntArr(val)

	for _, days := range arr {
		if domain.DaysToExpires == days {
			return false
		}
	}

	if arr[0] > domain.DaysToExpires {
		arr = append(arr, domain.DaysToExpires)
		r.records[domain.Fqdn] = intArrToString(arr)
	} else {
		r.records[domain.Fqdn] = fmt.Sprintf("%d", domain.DaysToExpires)
	}

	return true
}
func strToIntArr(val string) []int {
	var out []int

	for _, dat := range strings.Split(val, ",") {
		int1, _ := strconv.ParseInt(dat, 10, 12)
		out = append(out, int(int1))
	}

	sort.SliceStable(out, func(i, j int) bool {
		return out[i] > out[j]
	})

	return out
}

func intArrToString(val []int) string {
	var out []string

	sort.SliceStable(val, func(i, j int) bool {
		return val[i] > val[j]
	})

	for _, dat := range val {
		out = append(out, fmt.Sprintf("%d", dat))
	}

	return strings.Join(out, ",")
}
