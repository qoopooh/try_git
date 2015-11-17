var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope) {
    $scope.firstName= "Berm";
    $scope.lastName= "Win";
    $scope.fullName = function() {
      return $scope.firstName + " " + $scope.lastName;
    }
    $scope.names=[{name:'เจนนี่',country:'เดนมาร์ก'},
    {name:'หิว',country:'นอร์เวย์'},
    {name:'ไก่',country:'สวีเดน'}];
});
