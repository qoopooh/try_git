package ssl

import (
	"crypto/tls"
	"fmt"
	"time"
)

type Result struct {
	Fqdn          string
	Expiry        time.Time
	DaysToExpires int
}

func Query(fqdn string, c chan<- Result) {

	conn, err := tls.Dial("tcp", fmt.Sprintf("%s:443", fqdn), nil)
	if err != nil {
		panic("Server doesn't support SSL certificate err: " + err.Error())
	}

	expiry := conn.ConnectionState().PeerCertificates[0].NotAfter
	diff := expiry.Sub(time.Now())

	c <- Result{
		Fqdn:          fqdn,
		Expiry:        expiry,
		DaysToExpires: int(diff.Hours() / 24),
	}
}
