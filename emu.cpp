#include <bits/stdc++.h>

using namespace std;

// Global variables for registers
int32_t A, B, SP, PC;

// Global memory array
int32_t memory[100000];

// Function to display usage information
void prompt()
{
    cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-trace\tshow instruction trace" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay ISA" << endl;
}

// Function to convert integer to hexadecimal
string int_to_hex(int i)
{
    stringstream stream;
    stream << setfill('0') << setw(8) << hex << i;
    return stream.str();
}

// Helper function to print a single memory line
void print_memory_line(int index, int memory_value, ofstream &trcfile, bool new_line = false)
{
    stringstream ss;
    // Print memory address and value with formatting
    ss << (new_line ? "\n\n" : "") << int_to_hex(index) << "\t" << int_to_hex(memory_value) << " ";
    cout << ss.str();    // Output to console
    trcfile << ss.str(); // Output to file
}

// Function to dump memory contents
void mem_dump(int poc, ofstream &trcfile)
{
    cout << "Dumping from memory" << endl;
    trcfile << "Dumping from memory" << endl;

    for (int i = 0; i < poc; i++)
    {
        // If this is the first byte in a new line (every 4th byte), print a new line before
        if (i % 4 == 0)
        {
            print_memory_line(i, memory[i], trcfile, true);
        }
        else
        {
            cout << int_to_hex(memory[i]) << " ";
            trcfile << int_to_hex(memory[i]) << " ";
        }
    }
    cout << endl;
}

// Function to retrieve operand and opcode from machine code
pair<int32_t, int32_t> retrieve_operand_opcode(int32_t instr)
{
    int32_t opcode = instr & 0xff;          // Masking first 6 bits (opcode)
    int32_t operand = (instr & 0xffffff00); // Masking last 2 bits and shifting operand
    operand >>= 8;
    return make_pair(operand, opcode); // Return as a pair
}

// Function to display the Instruction Set Architecture (ISA)
void Instruction_Set_Architecture()
{
    // Print a header
    cout << "Opcode  Mnemonic  Operand" << endl;
    cout << "-------------------------" << endl;

    // Listing the opcodes, mnemonics, and operands
    cout << "0       ldc        VALUE" << endl;
    cout << "1       adc        VALUE" << endl;
    cout << "2       ldl        VALUE" << endl;
    cout << "3       stl        VALUE" << endl;
    cout << "4       ldnl       VALUE" << endl;
    cout << "5       stnl       VALUE" << endl;
    cout << "6       add              " << endl;
    cout << "7       sub        " << endl;
    cout << "8       shl        " << endl;
    cout << "9       shr        " << endl;
    cout << "10      adj        VALUE" << endl;
    cout << "11      a2sp       " << endl;
    cout << "12      sp2a       " << endl;
    cout << "13      call       OFFSET" << endl;
    cout << "14      return     " << endl;
    cout << "15      brz        OFFSET" << endl;
    cout << "16      brlz       OFFSET" << endl;
    cout << "17      br         OFFSET" << endl;
    cout << "18      HALT       " << endl;
    cout << "19      data       VALUE" << endl;
    cout << "20      SET        VALUE" << endl;
}

// Machine Operation Table
map<int, string> mot;

// Function to initialize the Machine Operation Table (MOT)
void mot_init()
{
    // Array of strings corresponding to opcodes
    const string mot_names[] = {
        "ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr",
        "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT",
        "data", "SET"};

    // Insert into map using the index as the key
    for (int i = 0; i < 21; i++)
    {
        mot[i] = mot_names[i];
    }
}

void execute_operation(int operation, int operand, int &A, int &B, int &SP, int &PC);
// Function to print the trace of the current instruction
void print_trace(int PC, int SP, int A, int B, int operation, int operand, ofstream &trcfile)
{
    string trace_output = "PC: " + int_to_hex(PC) + "\tSP: " + int_to_hex(SP) + "\tA: " +
                          int_to_hex(A) + "\tB: " + int_to_hex(B) + "\t" + mot[operation] +
                          " " + to_string(operand) + "\n\n";
    cout << trace_output;    // Output to console
    trcfile << trace_output; // Output to file
}

// Function to trace the instructions during execution
int trace(int PC, int poc, ofstream &trcfile, bool should_trace)
{
    int count = 0;
    while (PC < poc)
    {
        int old_pc = PC;
        int32_t operand, operation;

        // Fetch instruction and decode operand and opcode
        int32_t instr = memory[PC];
        tie(operand, operation) = retrieve_operand_opcode(instr);

        // Print trace information
        if (should_trace)
        {
            print_trace(PC, SP, A, B, operation, operand, trcfile);
        }
        // Execute operation based on the opcode
        execute_operation(operation, operand, A, B, SP, PC);

        // Check for HALT condition
        if (operation == 18)
        {
            cout << count << " number of instructions executed" << endl;
            return 0;
        }

        // Increment PC and check for infinite loop anomaly
        PC++;
        if (old_pc == PC)
        {
            cout << "Infinite loop detected" << endl;
            break;
        }

        // Increment executed instruction count
        count++;
    }

    cout << count << " number of instructions executed" << endl;
    return count;
}

// Function to execute operations based on the opcode
void execute_operation(int operation, int operand, int &A, int &B, int &SP, int &PC)
{
    if (operation == 0)
    { // ldc
        B = A;
        A = operand;
    }
    else if (operation == 1)
    { // adc
        A += operand;
    }
    else if (operation == 2)
    { // ldl
        B = A;
        A = memory[SP + operand];
    }
    else if (operation == 3)
    { // stl
        memory[SP + operand] = A;
        A = B;
    }
    else if (operation == 4)
    { // ldnl
        A = memory[A + operand];
    }
    else if (operation == 5)
    { // stnl
        memory[A + operand] = B;
    }
    else if (operation == 6)
    { // add
        A += B;
    }
    else if (operation == 7)
    { // sub
        A = B - A;
    }
    else if (operation == 8)
    { // shl
        A = B << A;
    }
    else if (operation == 9)
    { // shr
        A = B >> A;
    }
    else if (operation == 10)
    { // adj
        SP += operand;
    }
    else if (operation == 11)
    { // a2sp
        SP = A;
        A = B;
    }
    else if (operation == 12)
    { // sp2a
        B = A;
        A = SP;
    }
    else if (operation == 13)
    { // call
        B = A;
        A = PC;
        PC += operand;
    }
    else if (operation == 14)
    { // return
        PC = A;
        A = B;
    }
    else if (operation == 15)
    { // brz
        if (A == 0)
        {
            PC += operand;
        }
    }
    else if (operation == 16)
    { // brlz
        if (A < 0)
        {
            PC += operand;
        }
    }
    else if (operation == 17)
    { // br
        PC += operand;
    }
    else if (operation == 18)
    { // HALT
        cout << "Execution complete." << endl;
    }
}

// Main function to run the emulator
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        prompt(); // Print usage prompt
        return 0;
    }

    // Initialize the machine operand table
    mot_init();

    // Argument for input file
    string in_file = argv[2];

    // Argument for output file
    string trace_file = in_file.substr(0, in_file.find(".", 0)) + ".trace";

    // Input object file
    ifstream infile;
    // Output trace file
    ofstream trcfile;

    infile.open(in_file, ios::in | ios::binary);
    if (!infile.is_open())
    {
        cerr << "Error opening input file: " << in_file << endl;
        return 1;
    }

    trcfile.open(trace_file);
    if (!trcfile.is_open())
    {
        cerr << "Error opening trace file: " << trace_file << endl;
        return 1;
    }

    // Load instructions into memory from file
    int32_t instr_code;
    int poc = 0;
    while (infile.read((char *)&instr_code, sizeof(instr_code)))
    {
        int32_t operand, operation;
        tie(operand, operation) = retrieve_operand_opcode(instr_code);

        // If instruction is DATA or SET, store the value into memory
        if (operation == 19 || operation == 20)
        {
            memory[poc++] = operand;
        }
        // Other instructions stored in memory
        else if (operation >= 0 && operation < 20)
        {
            memory[poc++] = instr_code;
        }
    }

    // Initialize registers
    A = 0;
    B = 0;
    PC = 0;
    SP = 0;
    bool should_trace = false; // flag to control whether to print trace or not.
    // Handle command-line options
    if (string(argv[1]) == "-before")
    {
        mem_dump(poc, trcfile); // Memory dump before execution
    }
    else if (string(argv[1]) == "-trace")
    {
        should_trace = true;                  // enable trace printing
        trace(0, poc, trcfile, should_trace); // Instruction trace
    }
    else if (string(argv[1]) == "-after")
    {
        should_trace = false;                 // disable trace printing
        trace(0, poc, trcfile, should_trace); // trace to change memory
        mem_dump(poc, trcfile);               // Memory dump after execution
    }
    else if (string(argv[1]) == "-isa")
    {
        Instruction_Set_Architecture(); // Display the ISA
    }

    infile.close();
    trcfile.close();

    return 0;
}
