enum PrinterError: Error {
    case outOfPaper
    case noToner
    case onFire
    case paperJam
    case noPower
    case invalidInput
    case printFailed
}

func send(job: Int, toPrinter printerName: String) throws -> String {

    if printerName == "Never Has Toner" {
        throw PrinterError.noToner
    } else if printerName == "Out of Paper" {
        throw PrinterError.outOfPaper
    } else if printerName == "Paper Jam" {
        throw PrinterError.paperJam
    } else if printerName == "No Power" {
        throw PrinterError.noPower
    } else if printerName == "Invalid Input" {
        throw PrinterError.invalidInput
    } else if printerName == "Print Failed" {
        throw PrinterError.printFailed
    }

    return "Job \(job) sent to printer \(printerName)"
}

do {
    let printerResponse = try send(job: 1040, toPrinter: "Never Has Toner")
    print(printerResponse)
} catch PrinterError.outOfPaper {
    print("Error: Out of paper.")
} catch PrinterError.noToner {
    print("Error: No toner.")
} catch PrinterError.paperJam {
    print("Error: Paper jam.")
} catch PrinterError.noPower {
    print("Error: No power.")
} catch PrinterError.invalidInput {
    print("Error: Invalid input.")
} catch PrinterError.printFailed {
    print("Error: Print failed.")
} catch {
    print("An unexpected error occurred: \(error).")
}


do {
    let printerResponse = try send(job: 1440, toPrinter: "Gutenberg")
    print(printerResponse)
} catch PrinterError.onFire {
    print("I'll just put this over here, with the rest of the fire.")
} catch let printerError as PrinterError {
    print("Printer error: \(printerError).")
} catch {
    print(error)
}
// Prints "Job sent"

let printerSuccess = try? send(job: 1884, toPrinter: "Mergenthaler")
let printerFailure = try? send(job: 1885, toPrinter: "Never Has Toner")
print(printerSuccess ?? "No job sent")
print(printerFailure ?? "No job sent")
