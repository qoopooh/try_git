protocol ExampleProtocol {
    var simpleDescription: String { get }
    mutating func adjust()
}

struct SimpleStructure: ExampleProtocol {
    var simpleDescription: String = "A very simple structure."
    var anotherProperty: Int = 69105
    mutating func adjust() {
        simpleDescription += " Now 100% adjusted."
    }
}

var a = SimpleStructure()
a.adjust()
let aDesc = a.simpleDescription
print(aDesc) // A very simple structure. Now 100% adjusted.

class SimpleClass: ExampleProtocol {
    var simpleDescription: String = "A very simple class."
    var anotherProperty: Int = 69106
    func adjust() {
        simpleDescription += " (adjusted)"
    }
}

var b = SimpleClass()
b.adjust()
let bDesc = b.simpleDescription
print(bDesc) // A very simple class. (adjusted)


// Extensions

extension Int: ExampleProtocol {
    var simpleDescription: String {
        return "The number \(self)"
    }
    
    mutating func adjust() {
        self += 42
    }
}

print(7.simpleDescription) // The number 7


let protocolValue: ExampleProtocol = a
print(protocolValue.simpleDescription) // A very simple structure. Now 100% adjusted.
