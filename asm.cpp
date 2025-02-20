/*
TITLE : claims
AUTHOR : M.V.D.SATHVIK
ROLL NO.: 2301AI34
*/

#include <bits/stdc++.h>

using namespace std;
// Define structure for symbols
struct symbol
{
    string name;
    int address;
    bool set;
    int set_value;
};

// Define structure for literals
struct literal
{
    int literal;
    int address;
};

// Symbol table
vector<symbol> sym_table;

// Literal table
vector<literal> lit_table;

std::string trim(const std::string &str)
{
    // Find the first non-whitespace character from the start
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
    {
        // If no non-whitespace character is found, return an empty string
        return "";
    }

    // Find the last non-whitespace character
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    // Return the substring that excludes leading and trailing spaces
    return str.substr(start, end - start + 1);
}

bool is_number(const std::string &s)
{
    // If the string is empty, it's not a valid number
    if (s.empty())
    {
        return false;
    }
    size_t start = 0;

    // Check for an optional '+' or '-' sign at the beginning
    if (s[start] == '+' || s[start] == '-')
    {
        start++;
    }

    // Check for hexadecimal (0x or 0X)
    if (s[start] == '0' && (s[start + 1] == 'x' || s[start + 1] == 'X'))
    {
        start += 2; // Skip over the "0x" or "0X"

        // If there's no number after "0x", it's not a valid hex number
        if (start == s.length())
        {
            return false;
        }
        // Check if the rest of the string contains only valid hexadecimal digits
        for (size_t i = start; i < s.length(); ++i)
        {
            if (!std::isxdigit(s[i]))
            {
                return false; // Invalid hex character
            }
        }

        return true; // Valid hexadecimal number
    }

    // Check for decimal numbers (no "0x", just digits)
    for (size_t i = start; i < s.length(); ++i)
    {
        if (!std::isdigit(s[i]))
        {
            return false; // Non-digit character encountered
        }
    }

    return true; // Valid decimal number
}

int tonum(const std::string &s)
{
    std::string trimmed = s;

    // Trim leading and trailing whitespace manually
    size_t start = 0;
    while (start < trimmed.length() && std::isspace(trimmed[start]))
        ++start;
    size_t end = trimmed.length();
    while (end > start && std::isspace(trimmed[end - 1]))
        --end;
    trimmed = trimmed.substr(start, end - start);

    // Handle empty string
    if (trimmed.empty())
    {
        throw std::invalid_argument("Input string is empty or only whitespace.");
    }

    bool isNegative = false;
    bool isHex = false;
    bool isOctal = false;
    size_t i = 0;

    // Handle negative numbers
    if (trimmed[0] == '-')
    {
        isNegative = true;
        ++i;
    }
    // Handle positive numbers
    else if (trimmed[0] == '+')
    {
        ++i;
    }

    // Handle hexadecimal (0x or 0X)
    if (i + 1 < trimmed.size() && trimmed[i] == '0' && (trimmed[i + 1] == 'x' || trimmed[i + 1] == 'X'))
    {
        isHex = true;
        i += 2; // Skip "0x" or "0X"
    }
    // Handle octal (starting with 0 but not 0x)
    else if (i < trimmed.size() && trimmed[i] == '0')
    {
        isOctal = true;
        ++i; // Skip the leading "0"
    }

    // Convert the number part
    int result = 0;

    if (isHex)
    {
        for (; i < trimmed.length(); ++i)
        {
            char c = trimmed[i];
            int digit;
            if (std::isdigit(c))
            {
                digit = c - '0';
            }
            else if (c >= 'a' && c <= 'f')
            {
                digit = c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F')
            {
                digit = c - 'A' + 10;
            }
            else
            {
                throw std::invalid_argument("Invalid character in hexadecimal string.");
            }
            result = result * 16 + digit;
        }
    }
    else if (isOctal)
    {
        for (; i < trimmed.length(); ++i)
        {
            char c = trimmed[i];
            int digit = c - '0';
            if (digit < 0 || digit > 7)
            {
                throw std::invalid_argument("Invalid character in octal string.");
            }
            result = result * 8 + digit;
        }
    }
    else
    {
        for (; i < trimmed.length(); ++i)
        {
            char c = trimmed[i];
            if (!std::isdigit(c))
            {
                throw std::invalid_argument("Invalid character in decimal string.");
            }
            result = result * 10 + (c - '0');
        }
    }

    // Apply negative sign if needed
    if (isNegative)
    {
        result = -result;
    }

    return result;
}

bool contains(const string &name)
{
    for (size_t i = 0; i < sym_table.size(); ++i)
    {
        if (sym_table[i].name == name)
        {
            return true;
        }
    }
    return false;
}

// Function to check if a literal exists in the literal table by value
bool contains(int literal_value)
{
    for (size_t i = 0; i < lit_table.size(); ++i)
    {
        if (lit_table[i].literal == literal_value)
        {
            return true;
        }
    }
    return false;
}

// Utility function to convert int to hex
string int_to_hex(int i)
{
    char buffer[9];             // 8 characters for hex + 1 for null terminator
    sprintf(buffer, "%08x", i); // Format integer as hex
    return string(buffer);
}
// Utility function for checking correct label name format
int isValidLabel(string label)
{
    // Check if the first character is a letter (A-Z or a-z)
    if (label.length() == 0)
    {
        return false; // Empty label is invalid
    }
    if (!((label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
    {
        return false; // The first character must be a letter
    }

    // Check the rest of the characters
    for (int i = 1; i < label.length(); i++)
    {
        char c = label[i]; // Get the current character

        // If the character is neither a letter nor a digit, it's an invalid label
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c = '_')))
        {
            return false; // Invalid character found
        }
    }

    return true; // The label is valid
}
// MOT table
map<string, string> mot;

void mot_init()
{
    mot = {
        {"ldc", "00"},

        {"adc", "01"},

        {"ldl", "02"},

        {"stl", "03"},

        {"ldnl", "04"},

        {"stnl", "05"},

        {"add", "06"},

        {"sub", "07"},

        {"shl", "08"},

        {"shr", "09"},

        {"adj", "0a"},

        {"a2sp", "0b"},

        {"sp2a", "0c"},

        {"call", "0d"},

        {"return", "0e"},

        {"brz", "0f"},

        {"brlz", "10"},

        {"br", "11"},

        {"HALT", "12"},

        {"data", "13"},

        {"SET", "14"}};
}

// Reading instructions and add into literal and symbol table

string inst_to_table(string instr, int *loc_ptr, int line)
{
    int loc = *loc_ptr;

    string errors = "";

    // Check if instruction contains a label (denoted by ":")

    size_t colon_pos = instr.find(":");

    if (colon_pos != string::npos)
    {
        string label = instr.substr(0, colon_pos);

        // Check for duplicate labels
        if (contains(label))
        {
            string err_msg = "ERROR: Duplicate Label at line " + to_string(line);
            cout << err_msg << endl;
            errors += err_msg + "\n";
        }

        // Validate the label format
        if (!isValidLabel(label))
        {
            string warn_msg = "WARNING: Incorrect label format at line " + to_string(line);
            cout << warn_msg << endl;
            errors += warn_msg + "\n";
        }

        // If instruction exists after the colon, process it
        if (colon_pos != instr.length() - 1)
        {
            string instruction_after_colon = instr.substr(colon_pos + 1);
            instruction_after_colon = trim(instruction_after_colon); // Remove leading/trailing spaces

            // Recursively process the instruction after the label
            inst_to_table(instruction_after_colon, &loc, line);

            // Split the instruction into operation and operand
            size_t space_pos = instruction_after_colon.find(" ");
            string operation = trim(instruction_after_colon.substr(0, space_pos));
            string operand = trim(instruction_after_colon.substr(space_pos + 1));

            // Handle 'SET' instruction
            if (operation == "SET")
            {
                sym_table.push_back({label, loc, 1, stoi(operand)});
            }
            else
            {
                sym_table.push_back({label, loc, 0, -1});
            }
        }
        else
        {
            // If no instruction follows the label, just add the label to the symbol table
            sym_table.push_back({label, loc, 0, -1});
            *loc_ptr = *loc_ptr - 1; // Adjust the location pointer
        }
    }
    else
    {
        // No label, process as literal or constant
        size_t space_pos = instr.find(" ");
        string operand = instr.substr(space_pos + 1);
        operand = trim(operand); // Remove leading/trailing spaces

        // Check if operand is a number (literal)
        if (is_number(operand))
        {
            lit_table.push_back({tonum(operand), -1});
        }
    }
    return errors;
}

void process_line(const string &line, int &loc, int line_count, ofstream &logfile)
{
    string instr = line.substr(0, line.find(";", 0)); // Remove comments
    instr = trim(instr);                              // Trim any excess spaces

    // Skip empty lines
    if (instr.empty())
    {
        return;
    }

    // Process the instruction and log errors or updates
    logfile << inst_to_table(instr, &loc, line_count);
    loc++; // Increment location after processing each line
}

void assign_literatures(int &loc)
{
    for (auto &lit : lit_table)
    {
        if (lit.address == -1)
        {
            lit.address = loc++; // Assign a location for unaddressed literals
        }
    }
}

void analyse(const string &file, ofstream &logfile)
{
    string line;
    int loc = 0;
    int line_count = 1;

    ifstream MyFile(file); // Open the input file

    if (!MyFile.is_open())
    {
        cerr << "Error: Could not open file " << file << endl;
        return; // Exit if the file can't be opened
    }

    // Process each line in the file
    while (getline(MyFile, line))
    {
        process_line(line, loc, line_count++, logfile); // Process and log the line
    }

    // Handle LTORG loop for literals after processing the file
    assign_literatures(loc);

    MyFile.close(); // Close the file after processing
}
tuple<string, string, string> inst_to_code(string instr, int *loc_ptr, int line)
{
    // Program Counter
    int loc = *loc_ptr;

    // Hex Code of Program Counter
    string encoding = int_to_hex(loc) + " ";

    // Warnings and errors
    string encoding_ = "";

    string errors = "";

    string machine_code = "";

    // Identify label and variables
    if (instr.find(':') != string::npos)
    {
        int colon = instr.find(":", 0);

        // Instruction could be present after the colon
        if (colon != instr.length() - 1)
        {
            string subs = instr.substr(colon + 1, instr.length());
            subs = trim(subs);
            tie(encoding_, errors, machine_code) = inst_to_code(subs, &loc, line);
            string temp = encoding_;
            temp = temp.substr(9, 9); // Remove address part
            encoding += temp;
        }
        // If no instruction after colon, PC shouldn't change
        else
        {
            encoding += "         ";
            *loc_ptr = *loc_ptr - 1;
        }
        encoding += instr + "\n";
    }

    // Identify literals and constants

    else
    {
        // Considering that second part of instruction is operand
        int space = instr.find(" ", 0);
        string sub_operand = "";
        if (space != string::npos)
        {
            sub_operand = trim(instr.substr(space + 1));
        }

        // String containing operation
        string sub_operation = instr.substr(0, space);

        sub_operand = trim(sub_operand);
        sub_operation = trim(sub_operation);

        // Checking for invalid mnemonics
        if (mot.find(sub_operation) == mot.end())
        {
            errors += "ERROR: Mnemonic not defined at line " + to_string(line) + "\n";
            cout << "ERROR: Mnemonic not defined at line " << line << endl;
        }

        // Checking for No operand instructions
        else if (sub_operation == "add" || sub_operation == "sub" || sub_operation == "shl" || sub_operation == "shr" || sub_operation == "a2sp" || sub_operation == "sp2a" || sub_operation == "return" || sub_operation == "HALT")
        {
            encoding += "000000" + mot[sub_operation] + " ";
            machine_code += "000000" + mot[sub_operation] + " ";
            if (!sub_operand.empty()) // Error: operand should not be present
            {
                errors += "ERROR: Operand not expected for '" + sub_operation + "' at line " + to_string(line) + "\n";
                cout << "ERROR: Operand not expected for '" + sub_operation + "' at line " << line << endl;
            }
        }
        // Handling `SET` or `data` instructions (no opcode, just value)
        else if (sub_operation == "SET" || sub_operation == "data")
        {
            string hex_string = int_to_hex(tonum(sub_operand)); // Convert the operand directly to hex
            encoding += hex_string + " ";                       // Only add the operand in encoding
            machine_code += hex_string + " ";                   // Same for machine code
        }
        // Checking for numeral operand to encode directly
        else if (is_number(sub_operand))
        {
            string hex_string = int_to_hex(tonum(sub_operand));
            encoding += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
            machine_code += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
        }
        // Checking for variable operand to encode address
        else
        {
            int defined = 0;
            for (int i = 0; i < sym_table.size(); i++)
            {
                if (sym_table[i].name.compare(sub_operand) == 0)
                {
                    defined = 1;
                    // SET variables considered numeral
                    string hex_string;
                    if (sym_table[i].set)
                    {
                        hex_string = int_to_hex(sym_table[i].set_value);
                    }
                    // Operands which need offset from PC
                    else if (sub_operation == "call" || sub_operation == "brz" || sub_operation == "brlz" || sub_operation == "br")
                    {
                        hex_string = int_to_hex(sym_table[i].address - loc - 1);
                    }
                    // Normal case, encode address
                    else
                    {
                        hex_string = int_to_hex(sym_table[i].address);
                    }
                    encoding += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
                    machine_code += hex_string.substr(hex_string.length() - 6) + mot[sub_operation] + " ";
                    break;
                }
            }
            if (sub_operand.length() == instr.length()) // No operand provided for an operand-required instruction
            {
                errors += "ERROR: Operand expected at line " + to_string(line) + "\n";
                cout << "ERROR: Operand expected at line " << line << endl;
            }
            else if (!defined)
            {
                errors += "ERROR: Unknown Symbol as operand at line " + to_string(line) + "\n";
                cout << "ERROR: Unknown Symbol as operand at line " << line << endl;
            }
        }
        encoding += instr + "\n"; // Add the original instruction to encoding
    }

    return make_tuple(encoding, errors, machine_code);
}

bool writeMachineCode(const string &machineCode, ofstream &objfile, int lineCount, ofstream &logfile);
void synthesize(const string &file, ofstream &outfile, ofstream &logfile, ofstream &objfile)
{
    // Open the input file
    ifstream inputFile(file);
    if (!inputFile.is_open())
    {
        logfile << "ERROR: Unable to open input file: " << file << endl;
        cout << "ERROR: Unable to open input file: " << file << endl;
        return;
    }

    string line;
    int loc = 0;       // Program counter
    int lineCount = 1; // Line counter for input

    bool hasErrors = false;

    // Read the input file line by line
    while (getline(inputFile, line))
    {
        // Pre-process the line: trim spaces and remove comments
        string instr = trim(line.substr(0, line.find(";")));
        if (instr.empty())
        {
            lineCount++;
            continue; // Skip empty lines or lines with only comments
        }

        // Process the instruction (encoding, errors, machine code)
        string encoding, errors, machineCode;
        tie(encoding, errors, machineCode) = inst_to_code(instr, &loc, lineCount++);
        lineCount++;

        // Write the encoded instruction to the listing file
        outfile << encoding;

        // If there are machine code instructions, write them to the object file
        if (!machineCode.empty())
        {
            hasErrors |= writeMachineCode(machineCode, objfile, lineCount, logfile);
        }

        // Log any errors generated by the instruction processing
        if (!errors.empty())
        {
            logfile << errors;
            hasErrors = true;
        }

        loc++; // Increment the program counter
    }

    // Final error status logging
    if (hasErrors)
    {
        cout << "Errors encountered during synthesis. Check the log file for details." << endl;
    }

    inputFile.close();
}
// Helper function to write the machine code to the object file
bool writeMachineCode(const string &machineCode, ofstream &objfile, int lineCount, ofstream &logfile)
{
    try
    {
        uint32_t machineCodeValue = stoul("0x" + trim(machineCode), nullptr, 16);
        objfile.write(reinterpret_cast<const char *>(&machineCodeValue), sizeof(machineCodeValue));
        return false; // No error occurred
    }
    catch (const invalid_argument &e)
    {
        logfile << "ERROR: Invalid machine code '" << machineCode << "' at line " << lineCount << "\n";
        cout << "ERROR: Invalid machine code '" << machineCode << "' at line " << lineCount << endl;
        return true; // An error occurred
    }
}

// Helper function to generate output file names based on the input file name
void generate_output_files(const string &input_file, string &out_file, string &log_file, string &obj_file)
{
    string base_name = input_file.substr(0, input_file.find("."));
    out_file = base_name + ".L";
    log_file = base_name + ".log";
    obj_file = base_name + ".o";
}

// Main function
int main()
{
    // Initialize Machine operation table
    mot_init();

    // Prompt user for input file
    string in_file;
    cout << "Enter the name of the assembly file: ";
    cin >> in_file;

    // Generate output file names
    string out_file, log_file, obj_file;

    generate_output_files(in_file, out_file, log_file, obj_file);

    // Open output, log, and object files
    ofstream outfile(out_file);
    ofstream logfile(log_file);
    ofstream objfile(obj_file, ios::out | ios::binary);

    // Check if the files were successfully opened
    if (!outfile.is_open() || !logfile.is_open() || !objfile.is_open())
    {
        cerr << "ERROR: Failed to open one or more output files!" << endl;
        return 1;
    }

    // Pass-1 of assembly: analysis phase
    analyse(in_file, logfile);

    // Pass-2 of assembly: synthesis phase
    synthesize(in_file, outfile, logfile, objfile);

    // Close all output files
    outfile.close();
    logfile.close();
    objfile.close();

    // Notify user about successful assembly completion
    cout << "Assembly completed. Output files generated: "
         << out_file << ", " << log_file << ", " << obj_file << endl;

    return 0;
}
