var fridgeIsOpen = false
var fridgeContents = ["milk", "eggs", "cheese"]

func fridgeContains(_ food: String) -> Bool {
    fridgeIsOpen = true
    defer {
        fridgeIsOpen = false
    }

    let result = fridgeContents.contains(food)
    return result
}

if fridgeContains("milk") {
    print("The fridge contains milk.")
} else {
    print("The fridge does not contain milk.")
}
print("Fridge is open: \(fridgeIsOpen)")
