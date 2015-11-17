var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope, $http) {
    $scope.firstName= "Berm";
    $scope.lastName= "Win";
    $scope.fullName = function() {
      return $scope.firstName + " " + $scope.lastName;
    }
    $scope.names=[{name:'เจนนี่',country:'เดนมาร์ก'},
    {name:'หิว',country:'นอร์เวย์'},
    {name:'ไก่',country:'สวีเดน'}];


    $http.get("http://www.w3schools.com/angular/customers.php")
      .success(function(response) {$scope.customers = response.records;});

    $scope.mySwitch = true;

    $scope.count = 0;
    $scope.buttonClicked = function() {
      return $scope.count += 1;
    }
}
);
