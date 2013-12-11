
function Gateway(address) {
  this.id = address;
  this.command = "E,V";
  this.phone_id = '0';
  this.date = new Date();
}

function Gws() {
  this.gws = [];
  this.version = Gws.CurrentVersion;
}

Gws.CurrentVersion = 1;
Gws.prototype.sync = function() {
  /*this.gws.forEach(function(gateway) {*/
  /*gateway.date = gateway.date.toJSON();*/
  /*});*/

  chrome.storage.sync.set({ 'gws': this });
  chrome.storage.sync.set({ 'current_gateway': current_gateway });
}
Gws.prototype.add = function(gateway) {
  this.gws.push(gateway);
  this.sync();
}
Gws.prototype.remove = function(gateway) {
  this.gws.splice(this.gws.indexOf(gateway), 1);
  this.sync();
}
Gws.prototype.length = function() {
  return this.gws.length;
}
Gws.prototype.findByKey = function(key, value) {
  for (var i = 0; i < this.gws.length; ++i) {
    var gateway = this.gws[i];
    if (gateway[key] === value)
      return gateway;
  }
  return null;
}
Gws.prototype.findById = function(value) {
  return this.findByKey("id", value);
}
Gws.prototype.sortedByKey = function(key) {
  return this.gws.slice(0).sort(function(a,b){
    var ret = (typeof a[key] === 'string') ? a[key].localeCompare(b[key]) : a[key] - b[key];
    return ret;
  });
}
Gws.prototype.ordered = function() {
  return this.sortedByKey('date');
}
Gws.prototype.asArray = function() {
  return this.gws;
}
function GatewayData(host, cmd, phone_id) {
  this.id = host;
  this.command = cmd;
  this.phone_id = phone_id;
}

var gws = null;
var current_gateway = "192.168.1.39";
var gateway_data = {}; // map gateway.id->GatewayData

function getCurrentGateway() {
  var gateway = gws.findById(current_gateway);
  if (!gateway) {
    gateway = new Gateway(current_gateway);
    gws.add(gateway);
  }
  return gateway;
}
function selectGateway(gateway) {
  $("#ip").val(gateway.id);
  $("#command").val(gateway.command);
}
