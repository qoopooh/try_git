package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strings"
)

// Dictionary for translation
var dictionary = map[string]string{
	"program": "โปรแกรม",
	"compile": "คอมไฟล์",
	"debug":   "ดีบัก",
	"test":    "ทดสอบ",
}

func genregexPattern() string {
	var words []string
	for word := range dictionary {
		words = append(words, word)
	}
	return fmt.Sprintf(`(?i)\s*(%s)\s*`, strings.Join(words, "|"))
}

// Function to process a line from the .md file
func processLine(line string, lineNumber int, changes *[]string) string {
	pattern := genregexPattern()
	re := regexp.MustCompile(pattern)
	return re.ReplaceAllStringFunc(line, func(match string) string {
		word := strings.TrimSpace(strings.ToLower(match))
		if replacement, exists := dictionary[word]; exists {
			trimmedMatch := strings.TrimSpace(match)
			*changes = append(*changes, fmt.Sprintf("%s -> %s ที่บรรทัดที่ %d", trimmedMatch, replacement, lineNumber))
			return replacement
		}
		return match
	})
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("กรุณาระบุไฟล์ .md ที่ต้องการจะแก้ไข")
		return
	}

	// When no output file is specified, use the default output file name
	inputFileName := os.Args[1]
	outputFileName := "out-" + inputFileName
	if len(os.Args) > 2 {
		outputFileName = os.Args[2]
	}

	// Open input file
	inputFile, err := os.Open(inputFileName)
	if err != nil {
		fmt.Println("ไม่สามารถเปิดไฟล์ได้:", err)
		return
	}
	defer inputFile.Close()

	// Create/truncate output file
	outputFile, err := os.Create(outputFileName)
	if err != nil {
		fmt.Println("ไม่สามารถสร้างหรือเขียนทับไฟล์ปลายทางได้:", err)
		return
	}
	defer outputFile.Close()

	outputWriter := bufio.NewWriter(outputFile)

	scanner := bufio.NewScanner(inputFile)
	changes := []string{}
	processedLines := []string{}
	lineNumber := 1

	for scanner.Scan() {
		line := scanner.Text()
		processedLine := processLine(line, lineNumber, &changes)
		processedLines = append(processedLines, processedLine)
		lineNumber++
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("เกิดข้อผิดพลาดในการอ่านไฟล์ต้นทาง:", err)
		return
	}

	// Write processed lines to output file
	for _, line := range processedLines {
		_, err := outputWriter.WriteString(line + "\n")
		if err != nil {
			fmt.Println("เกิดข้อผิดพลาดในการเขียนไปยังไฟล์ปลายทาง:", err)
			return
		}
	}

	err = outputWriter.Flush()
	if err != nil {
		fmt.Println("เกิดข้อผิดพลาดในการ flush ข้อมูลไปยังไฟล์ปลายทาง:", err)
		return
	}

	// Write changes to standard output
	fmt.Println("รายการแก้ไข:")
	for _, change := range changes {
		fmt.Println(change)
	}

	fmt.Println("ประมวลผลไฟล์สำเร็จ บันทึกผลลัพธ์ไปยัง:", outputFileName)
}
