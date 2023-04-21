type SimpleUser = {
    name: string
}

type SpecialAdmin = {
    name: string
    isAdmin: boolean
}

/**
 * Check admin
 * @param account user or admin
 *
 * @returns admin status
 */
function isAdmin(account: SimpleUser | SpecialAdmin): boolean {
    if ("isAdmin" in account) {
        return account.isAdmin;
    }

    return false;
}


function logValue(x: Date | string) {
    if (x instanceof Date) {
        console.log(x.toUTCString());
    } else {
        console.log(x.toUpperCase());
    }
}


interface Circle {
    kind: "circle";
    radius: number;
}
   
interface Square {
    kind: "square";
    sideLength: number;
}
  
type Shape = Circle | Square;
 
function getArea(shape: Shape) {
  switch (shape.kind) {
    case "circle":
      return Math.PI * shape.radius ** 2;
    case "square":
      return shape.sideLength ** 2;
    default:
      const _exhaustiveCheck: never = shape;
      return _exhaustiveCheck;
  }
}