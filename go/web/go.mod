module berm.com/web

go 1.18

require (
	github.com/gorilla/websocket v1.5.0
  th.berm.trace/trace v0.0.0
)
replace (
  th.berm.trace/trace v0.0.0 => "../trace"
)
