// https://www.youtube.com/watch?v=d56mG7DezGs

let sales = 123_456_789;
let numbers = [1, 2, 3];
let user: [number, string] = [1, "Berm"]; // a pair is the best
let age: number = 20;
if (age < 50)
    age += 10;
console.log(`age ${age}`);

// PascalCase
const enum Size { Small = 1, Medium, Large };
let mySize = Size.Large;
console.log(`my size: ${mySize}`);

function calculateTax(income: number, taxYear: number = 2022): number {
    if (taxYear < 2022)
        return income * 1.2;

    return income * 1.3;
}

type Employee = {
    readonly id: number,
    name: string,
    dept?: string,
}

let employee: Employee = { id: 1, name: "Bob" };

employee.dept = "Account";


// union type
function kgToLbs(weight: number | string): number {
    if (typeof weight === 'number')
        return weight * 2.2;

    return parseFloat(weight) * 2.2;
}

kgToLbs(10);
console.log(kgToLbs("22.2kg"));


// Intersection types
type Draggable = {
    drag: () => void
}

type Resizeable = {
    resize: () => void
}

type UIWidget = Draggable & Resizeable;

let textBox: UIWidget = {
    drag: () => {},
    resize: () => {},
}

// Literal (exact, specific)
type Quantity = 50 | 100
let quantity: Quantity = 100;

type Metric = 'cm' | 'inch';

// Optional

type Customer = {
    birthday?: Date
}

function getCustomer(id: number): Customer | null | undefined {
    return id === 0 ? null :  {birthday: new Date() };
}

let customer = getCustomer(1);
console.log(customer?.birthday?.getFullYear());

// Optional element access operator
// customers?.[0]

// Optional call
let log: any = null;
log?.('a');