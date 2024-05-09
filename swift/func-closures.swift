func greet(person: String, day: String) -> String {
    return "Hello \(person), today is \(day)."
}
let msg1 = greet(person: "Bob", day: "Tuesday")
print(msg1)

// let msg2 = greet("Bob2", "Monday")
// print(msg2)  // does not work

func greet(_ person: String, on day: String) -> String {
    return "Hello \(person), today is \(day)."
}
let msg3 = greet("John", on: "Wednesday")
print(msg3)

func calculateStatistics(scores: [Int]) -> (min: Int, max: Int, sum: Int) {
    var min = scores[0]
    var max = scores[0]
    var sum = 0
    for score in scores {
        if score > max {
            max = score
        } else if score < min {
            min = score
        }
        sum += score
    }
    return (min, max, sum)
}

let statistics = calculateStatistics(scores: [5, 3, 100, 3, 9])
print(statistics.min)
print(statistics.max)
print(statistics.sum)
print(statistics)
print(statistics.2)


// Nested functions
func returnFifteen() -> Int {
    var y = 10
    func add() {
        y += 5
    }
    add()
    return y
}
let y = returnFifteen()
print(y)

// Returning functions
func makeIncrementer() -> ((Int) -> Int) {
    func addOne(number: Int) -> Int {
        return 1 + number
    }
    return addOne
}
let increment = makeIncrementer()
let incrementedValue = increment(7)
print(incrementedValue)

// Function as argument
func hasAnyMatches(list: [Int], condition: (Int) -> Bool) -> Bool {
    for item in list {
        if condition(item) {
            return true
        }
    }
    return false
}
func lessThanTen(number: Int) -> Bool {
    return number < 10
}
let numbers = [20, 19, 7, 12]
let result = hasAnyMatches(list: numbers, condition: lessThanTen)
print("hasAnyMatches:", result)

// Closure map
let mappedNumbers = numbers.map { (number: Int) -> Int in
    let result = 3 * number
    return result
}
print("mappedNumbers:", mappedNumbers)

// Sorting
let sortedNumbers = numbers.sorted {
    $0 > $1
}
print("sortedNumbers:", sortedNumbers)

let sortedNumbers2 = numbers.sorted(by: { (first: Int, second: Int) -> Bool in
    return first < second
})
print("sortedNumbers2:", sortedNumbers2)

let sortedNumbers3 = mappedNumbers.sorted { (first: Int, second: Int) -> Bool in
    //first *= 20 // 'first' is a 'let' constant
    first < second
}
print("sortedNumbers3:", sortedNumbers3)
