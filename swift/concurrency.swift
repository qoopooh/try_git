
func fetchUserID(from server: String) async -> Int {
    if server == "primary" {
        return 97
    }
    return 501
}


func fetchUsername(from server: String) async -> String {
    let userID = await fetchUserID(from: server)
    if userID == 501 {
        return "John Appleseed"
    }
    return "Guest"
}

func connectUser(to server: String) async {
    async let userID = fetchUserID(from: server)
    async let username = fetchUsername(from: server)

    let greeting = await "Hello \(username) with ID \(userID)"
    print(greeting)
}

Task {
    await connectUser(to: "primary")
}

let userIDs = await withTaskGroup(of: Int.self) { group in
    for server in ["primary", "secondary", "tertiary"] {
        group.addTask {
            await fetchUserID(from: server)
        }
    }

    var ids = [Int]()
    for await id in group {
        ids.append(id)
    }
    return ids
}

print(userIDs)


actor ServerConnection {
    let server: String = "primary"
    private var activeUsers: [Int] = []

    func connect() async -> Int {
        let userID = await fetchUserID(from: server)
        activeUsers.append(userID)
        return userID
    }
}

let server = ServerConnection()
let userID = await server.connect()
print("Connected user ID: \(userID)")
