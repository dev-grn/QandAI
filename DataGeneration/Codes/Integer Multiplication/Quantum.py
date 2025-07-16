#!/usr/bin/env python3
"""
Quantum Multiplication Calculator using Quantum Fourier Transform (QFT)

This module implements quantum multiplication using the QFT approach, which performs
multiplication by repeated addition in the quantum Fourier domain.

Required Environment and Library Versions:
- Python 3.8+
- qiskit==0.46.1 (specific version required for compatibility)
- qiskit_aer==0.13 (for local quantum simulation)
- pylatexenc (for circuit visualization)
- matplotlib (for plotting)
- math (standard library)

Installation commands:
pip3 install qiskit==0.46.1
pip3 install qiskit_aer==0.13
pip3 install pylatexenc
pip3 install matplotlib

Note: The multiplication algorithm works by:
1. Converting the result register to QFT domain
2. Repeatedly adding the first operand to the result register (second operand times)
3. Converting back from QFT domain to get the final result

The multiplication process requires addition functionality, so the QFT-based addition
functions are preserved in this implementation.
"""

import math
import qiskit
from qiskit import *
from qiskit_aer import Aer

# Global constant for pi
pie = math.pi

class bcolors:
    """Color codes for terminal output formatting"""
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def executeQFT(qc, reg, n, pie):
    """
    Computes the quantum Fourier transform of reg, one qubit at a time.
    Apply one Hadamard gate to the n-th qubit of the quantum register reg, and 
    then apply repeated phase rotations with parameters being pi divided by 
    increasing powers of two.
    
    Args:
        qc: Quantum circuit
        reg: Quantum register to apply QFT to
        n: Index of the qubit to process
        pie: Pi constant
    """
    qc.h(reg[n])    
    for i in range(0, n):
        qc.cp(pie/float(2**(i+1)), reg[n-(i+1)], reg[n])    


def inverseQFT(qc, reg, n, pie):
    """
    Performs the inverse quantum Fourier transform on a register reg.
    Apply repeated phase rotations with parameters being pi divided by 
    decreasing powers of two, and then apply a Hadamard gate to the nth qubit
    of the register reg.
    
    Args:
        qc: Quantum circuit
        reg: Quantum register to apply inverse QFT to
        n: Index of the qubit to process
        pie: Pi constant
    """
    for i in range(n):
        qc.cp(-1*pie/float(2**(n-i)), reg[i], reg[n])
    qc.h(reg[n])


def initQubits(str_bits, qc, reg, n):
    """
    Flip the corresponding qubit in register if a bit in the string is a 1.
    
    Args:
        str_bits: Binary string representation of the number
        qc: Quantum circuit
        reg: Quantum register to initialize
        n: Number of bits
    """
    for i in range(n):
        if str_bits[i] == "1":
            qc.x(reg[n-(i+1)])


def evolveQFTStateSum(qc, reg_a, reg_b, n, pie):
    """
    Evolves the state |F(ψ(reg_a))> to |F(ψ(reg_a+reg_b))> using the quantum 
    Fourier transform conditioned on the qubits of the reg_b.
    Apply repeated phase rotations with parameters being pi divided by 
    increasing powers of two.
    
    Args:
        qc: Quantum circuit
        reg_a: First quantum register (in QFT domain)
        reg_b: Second quantum register (classical state)
        n: Index of the qubit to process
        pie: Pi constant
    """
    l = len(reg_b)
    for i in range(n+1):
        if (n - i) > l - 1:
            pass
        else:
            qc.cp(pie/float(2**(i)), reg_b[n-i], reg_a[n])


def sum_qft(a, b, qc):
    """
    Quantum addition using QFT.
    This function is required for the multiplication algorithm.
    
    Args:
        a: First quantum register (result will be stored here)
        b: Second quantum register
        qc: Quantum circuit
    """
    n = len(a)-1
    # Compute the Fourier transform of register a
    for i in range(n+1):
        executeQFT(qc, a, n-i, pie)
        
    # Add the two numbers by evolving the Fourier transform F(ψ(reg_a))>
    # to |F(ψ(reg_a+reg_b))>
    for i in range(n+1):
        evolveQFTStateSum(qc, a, b, n-i, pie) 
        
    # Compute the inverse Fourier transform of register a
    for i in range(n+1):
        inverseQFT(qc, a, i, pie)


def multiply(a, secondDec, result, qc):
    """
    Quantum multiplication using QFT and repeated addition.
    
    The multiplication algorithm works by:
    1. Converting the result register to QFT domain
    2. Repeatedly adding register 'a' to the result register 'secondDec' times
    3. Converting back from QFT domain to get the final result
    
    Args:
        a: First quantum register (multiplicand)
        secondDec: Integer value of the second operand (multiplier)
        result: Quantum register to store the result
        qc: Quantum circuit
    """
    n = len(a) - 1
    # Compute the Fourier transform of register 'result'
    for i in range(n+1):
        executeQFT(qc, result, n-i, pie)
    
    # Add the two numbers by evolving the Fourier transform F(ψ(result))>
    # to |F(ψ((secondDec * reg_a))>, where we loop on the sum as many times as 'secondDec' says, 
    # doing incremental sums
    for j in range(secondDec):
        for i in range(n+1):
            evolveQFTStateSum(qc, result, a, n-i, pie)

    # Compute the inverse Fourier transform of register result
    for i in range(n+1):
        inverseQFT(qc, result, i, pie)


def printResult(first, second, qc, result, cl, n, operator):
    """
    Execute the quantum circuit and print the result.
    
    Args:
        first: Binary string of first operand
        second: Binary string of second operand
        qc: Quantum circuit
        result: Quantum register containing the result
        cl: Classical register for measurement
        n: Number of bits
        operator: String representation of the operation
    """
    # Measure qubits
    for i in range(n+1):
        qc.measure(result[i], cl[i])

    # Execute using the local simulator
    print(bcolors.BOLD + bcolors.OKCYAN + 'Connecting to local simulator...' + bcolors.ENDC)
    # Set chosen backend and execute job
    num_shots = 100  # Setting the number of times to repeat measurement
    print(bcolors.BOLD + bcolors.OKCYAN + 'Connected!' + bcolors.ENDC)
    print(bcolors.BOLD + bcolors.OKCYAN + f'Running the experiment on {num_shots} shots...' + bcolors.ENDC)
    job = execute(qc, backend=Aer.get_backend('qasm_simulator'), shots=num_shots)
    # Get results of program
    job_stats = job.result().get_counts()
    for key, value in job_stats.items():
        res = key
        prob = value
    print(bcolors.BOLD + bcolors.OKGREEN + f'\n{first} {operator} {second} = {res} with a probability of {prob}%' + bcolors.ENDC)


def quantum_multiplication():
    """
    Main function to perform quantum multiplication.
    Takes user input for two integers and computes their product using quantum circuits.
    """
    print(bcolors.BOLD + bcolors.HEADER + "Quantum Multiplication Calculator" + bcolors.ENDC)
    print(bcolors.BOLD + "Using Quantum Fourier Transform (QFT) approach" + bcolors.ENDC)
    print(f"Qiskit version: {qiskit.__version__}")
    print()
    
    # Get user input
    input1 = int(input(bcolors.WARNING + "Enter a first positive integer between 0 and 2047:\n" + bcolors.ENDC))
    input2 = int(input(bcolors.WARNING + "Enter a second positive integer between 0 and 2047:\n" + bcolors.ENDC))

    # Validate inputs
    while (input1 < 0 or input1 > 2047) or (input2 < 0 or input2 > 2047):
        if input1 < 0 or input1 > 2047:
            print(bcolors.FAIL + "Invalid first input number" + bcolors.ENDC)
            input1 = int(input(bcolors.WARNING + "Enter a first positive integer between 0 and 2047:\n" + bcolors.ENDC))

        if input2 < 0 or input2 > 2047:
            print(bcolors.FAIL + "Invalid second input number" + bcolors.ENDC)
            input2 = int(input(bcolors.WARNING + "Enter a second positive integer between 0 and 2047:\n" + bcolors.ENDC))

    # Convert to binary strings
    first = '{0:{fill}3b}'.format(input1, fill='0')
    second = '{0:{fill}3b}'.format(input2, fill='0')
    
    # Store decimal values for multiplication
    firstDec = input1
    secondDec = input2

    l1 = len(first)
    l2 = len(second)

    # Padding 'first' to the same length as 'result'
    # since result can have at max len(first) + len(second) bits when multiplying
    first = ("0")*(l2) + first
    n = l1 + l2

    # Create quantum registers
    a = QuantumRegister(n+1, "a")  # First operand
    b = QuantumRegister(n+1, "b")  # Second operand (result register)
    cl = ClassicalRegister(n+1, "cl")  # Classical register for measurement

    # Create quantum circuit
    qc = QuantumCircuit(a, b, cl, name="multiplication_circuit")
    
    # Initialize the first operand register
    initQubits(first, qc, a, n)

    # Perform multiplication
    multiply(a, secondDec, b, qc)
    
    # Print result
    operator = '*'
    printResult(first, second, qc, b, cl, n, operator)
    
    # Print circuit information
    print(f"\nCircuit depth: {qc.depth()}")
    print(f"Number of qubits: {qc.num_qubits}")
    print(f"Number of classical bits: {qc.num_clbits}")
    
    return qc


if __name__ == "__main__":
    # Run the quantum multiplication
    circuit = quantum_multiplication()
    
    # Optionally, you can draw the circuit (requires matplotlib and pylatexenc)
    try:
        print(bcolors.BOLD + bcolors.OKCYAN + "\nCircuit diagram available. Run circuit.draw('mpl') to visualize." + bcolors.ENDC)
    except ImportError:
        print(bcolors.WARNING + "Install matplotlib and pylatexenc to visualize the circuit." + bcolors.ENDC)
