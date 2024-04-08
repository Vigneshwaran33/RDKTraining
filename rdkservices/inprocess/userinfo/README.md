-----------------
Build:

bitbake thunder-plugins

-----------------
Test:

curl -d '{"jsonrpc":"2.0","id":"1","method":"Controller.1.activate","params":{"callsign":"org.rdk.UserInfo"}}' http://127.0.0.1:9998/jsonrpc

curl -d '{"jsonrpc":"2.0","id":"3","method": "org.rdk.UserInfo.1.setUserName", "params": {"user_name": "tony"}}' http://127.0.0.1:9998/jsonrpc

curl -d '{"jsonrpc":"2.0","id":"3","method": "org.rdk.UserInfo.1.getUserName", "params": {"user_name": "tony"}}' http://127.0.0.1:9998/jsonrpc