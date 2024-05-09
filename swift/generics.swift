func makeArray<Item>(repeating item: Item, count: Int) -> [Item] {
    var result = [Item]()
    for _ in 0..<count {
        result.append(item)
    }
    return result
}
let arrInt = makeArray(repeating: 3, count: 5) // [3, 3, 3, 3, 3]
let arrStr = makeArray(repeating: "hello", count: 3) // ["hello", "hello", "hello"]
print(arrInt)
print(arrStr)


enum OptionalValue<T> {
    case none
    case some(T)
}
var optionalInt: OptionalValue<Int> = .none
print(optionalInt) // none
optionalInt = .some(42)
print(optionalInt) // some(42)


func anyCommonElements<T: Sequence, U: Sequence>(_ lhs: T, _ rhs: U) -> Bool
    where T.Element: Equatable, T.Element == U.Element
{
    for lhsItem in lhs {
        for rhsItem in rhs {
            if lhsItem == rhsItem {
                return true
            }
        }
    }
   return false
}
let out = anyCommonElements([1, 2, 3], [3])
print(out) // true

