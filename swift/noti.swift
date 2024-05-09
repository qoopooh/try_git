import Foundation
import FoundationNetworking

let VERSION = "0.5"
let DEFAULT_SLACK_URL = "https://hooks.slack.com/services/XX/YY/ZZ"

// Get Slack URL from environment variable or use default
func getSlackWebhookURL() -> String {
    if let envURL = ProcessInfo.processInfo.environment["SLACK_WEBHOOK_URL"] {
        return envURL
    }
    return DEFAULT_SLACK_URL
}

func getAgent(fromOrigin origin: String? = nil) -> String {
    let processName = origin != nil ? URL(fileURLWithPath: origin!).lastPathComponent : URL(fileURLWithPath: CommandLine.arguments[0]).lastPathComponent
    let machine = ProcessInfo.processInfo.hostName
    let system = ProcessInfo.processInfo.operatingSystemVersionString
    
    return "\(processName) (\(machine)-\(system))"
}

func send(message: String, token: String? = nil, origin: String? = nil) async throws -> Data {
    let agent = origin != nil ? getAgent(fromOrigin: origin) : getAgent()
    let formattedMessage = "\(agent): \(message)"
    
    return try await slackWebhook(message: formattedMessage)
}

func slackWebhook(message: String) async throws -> Data {
    let slackURL = getSlackWebhookURL()
    
    guard let url = URL(string: slackURL) else {
        throw NSError(domain: "InvalidURL", code: 1, userInfo: [NSLocalizedDescriptionKey: "Invalid Slack URL"])
    }
    
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.addValue("application/json", forHTTPHeaderField: "Content-Type")
    
    let payload = ["text": message]
    let jsonData = try JSONSerialization.data(withJSONObject: payload)
    request.httpBody = jsonData
    
    let (data, response) = try await URLSession.shared.data(for: request)
    
    guard let httpResponse = response as? HTTPURLResponse, httpResponse.statusCode == 200 else {
        throw NSError(domain: "HTTPError", code: (response as? HTTPURLResponse)?.statusCode ?? 0, 
                    userInfo: [NSLocalizedDescriptionKey: "Failed to send message to Slack"])
    }
    
    return data
}

if CommandLine.arguments.count < 2 {
    print("Usage: \(CommandLine.arguments[0]) <message>")
    exit(1)
}

// Main execution
Task {
    do {
        let message = CommandLine.arguments[1]
        let _ = try await send(message: message)
        print("Message sent successfully")
    } catch {
        print("Error: \(error.localizedDescription)")
    }
    
    // Ensure the program exits after task completes
    exit(0)
}

// Keep the program running until the task completes
dispatchMain() 
