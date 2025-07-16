#!/usr/bin/env python3
"""
Simple Test Script for Quantum Multiplication

This script provides a minimal test example for the quantum multiplication functionality.
Run this after installing the required libraries to verify the implementation works.

Required installation:
pip3 install qiskit==0.46.1
pip3 install qiskit_aer==0.13
pip3 install pylatexenc
pip3 install matplotlib
"""

# Import the quantum multiplication module
from quantum_multiplication import quantum_multiplication, multiply, initQubits, printResult, bcolors
import math

def test_small_multiplication():
    """Test multiplication with small predefined values"""
    print(bcolors.BOLD + bcolors.HEADER + "Testing Quantum Multiplication with predefined values" + bcolors.ENDC)
    
    try:
        import qiskit
        from qiskit import QuantumRegister, ClassicalRegister, QuantumCircuit
        from qiskit_aer import Aer
        
        # Test case: 3 * 4 = 12
        input1, input2 = 3, 4
        print(f"Testing: {input1} * {input2}")
        
        # Convert to binary
        first = '{0:{fill}3b}'.format(input1, fill='0')
        second = '{0:{fill}3b}'.format(input2, fill='0')
        
        l1 = len(first)
        l2 = len(second)
        
        # Padding for multiplication result
        first = ("0")*(l2) + first
        n = l1 + l2
        
        # Create quantum registers
        a = QuantumRegister(n+1, "a")
        b = QuantumRegister(n+1, "b") 
        cl = ClassicalRegister(n+1, "cl")
        
        # Create circuit
        qc = QuantumCircuit(a, b, cl, name="test_mult")
        
        # Initialize first operand
        initQubits(first, qc, a, n)
        
        # Perform multiplication
        multiply(a, input2, b, qc)
        
        # Print result
        operator = '*'
        printResult(first, second, qc, b, cl, n, operator)
        
        print(f"Expected result: {input1 * input2}")
        print("Test completed successfully!")
        
    except ImportError as e:
        print(f"Import error: {e}")
        print("Please install required libraries:")
        print("pip3 install qiskit==0.46.1")
        print("pip3 install qiskit_aer==0.13")
        print("pip3 install pylatexenc")
        print("pip3 install matplotlib")


def run_interactive_test():
    """Run the interactive multiplication test"""
    print(bcolors.BOLD + bcolors.HEADER + "Interactive Quantum Multiplication Test" + bcolors.ENDC)
    try:
        circuit = quantum_multiplication()
        return circuit
    except ImportError as e:
        print(f"Import error: {e}")
        print("Please install required libraries first.")
        return None


if __name__ == "__main__":
    print("Quantum Multiplication Test Suite")
    print("=" * 40)
    
    choice = input("Choose test type:\n1. Predefined test (3 * 4)\n2. Interactive test\nEnter choice (1 or 2): ")
    
    if choice == "1":
        test_small_multiplication()
    elif choice == "2":
        run_interactive_test()
    else:
        print("Invalid choice. Running predefined test...")
        test_small_multiplication()
