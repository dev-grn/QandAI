#!/usr/bin/env python3

'''
/********************************************************************************************
 *       Shor's Algorithm (Demonstration with 15, modifiable to accept user input.)         *
 *                                    Created by QandAI                                     *
 *******************************************************************************************/
 '''

import numpy as np
from qiskit import QuantumCircuit, Aer, execute
from qiskit.circuit.library import QFT
from qiskit.visualization import plot_histogram
from math import gcd
import pandas as pd

def initialize_qubits(given_circuit, n, m):
    """Initialize qubits for Shor's algorithm"""
    given_circuit.h(range(n))
    given_circuit.x(n+m-1)

def c_amod15(a, x):
    """
    Controlled modular exponentiation for a^x mod 15
    Only works for a in [2,7,8,11,13]
    """
    if a not in [2,7,8,11,13]:
        raise ValueError("'a' must be 2,7,8,11,13")
    
    U = QuantumCircuit(4)        
    for iteration in range(x):
        if a in [2,13]:
            U.swap(0,1)
            U.swap(1,2)
            U.swap(2,3)
        if a in [7,8]:
            U.swap(2,3)
            U.swap(1,2)
            U.swap(0,1)
        if a == 11:
            U.swap(1,3)
            U.swap(0,2)
        if a in [7,11,13]:
            for q in range(4):
                U.x(q)
    
    U = U.to_gate()
    U.name = "%i^%i mod 15" % (a, x)
    c_U = U.control()
    return c_U

def modular_exponentiation(circuit, n, m, a):
    """Apply controlled modular exponentiation"""
    for x in range(n):
        exponent = 2**x
        circuit.append(c_amod15(a, exponent), 
                     [x] + list(range(n, n+m)))

def inverse_qft(circuit, measurement_qubits):
    """Apply inverse Quantum Fourier Transform"""
    circuit.append(QFT(len(measurement_qubits), do_swaps=False).inverse(), measurement_qubits)

def shors_algorithm(n, m, a):
    """
    Main Shor's algorithm implementation
    
    Args:
        n: Number of qubits for counting register
        m: Number of qubits for auxiliary register
        a: Base for modular exponentiation
    
    Returns:
        QuantumCircuit: Complete Shor's algorithm circuit
    """
    qc = QuantumCircuit(n+m, n)
    initialize_qubits(qc, n, m)
    qc.barrier()
    modular_exponentiation(qc, n, m, a)
    qc.barrier()
    inverse_qft(qc, range(n))
    qc.measure(range(n), range(n))
    return qc

def run_shors_algorithm(n=4, m=4, a=7, shots=1000):
    """
    Run Shor's algorithm and analyze results
    
    Args:
        n: Number of counting qubits
        m: Number of auxiliary qubits
        a: Base for modular exponentiation
        shots: Number of circuit executions
    
    Returns:
        dict: Measurement counts
    """
    print(f"Running Shor's algorithm to factor 15 with a={a}")
    print(f"Using {n} counting qubits and {m} auxiliary qubits")
    print(f"Number of shots: {shots}")
    print("-" * 50)
    
    # Create and run the circuit
    final_circuit = shors_algorithm(n, m, a)
    
    # Print circuit diagram
    print("Circuit diagram:")
    print(final_circuit)
    print("-" * 50)
    
    # Run simulation
    simulator = Aer.get_backend('qasm_simulator')
    counts = execute(final_circuit, backend=simulator, shots=shots).result().get_counts(final_circuit)
    
    return counts, final_circuit

def analyze_results(counts, a=7):
    """
    Analyze the measurement results to find factors
    
    Args:
        counts: Dictionary of measurement counts
        a: Base used in modular exponentiation
    """
    print("Measurement results:")
    measured_values = []
    
    for measured_value in counts:
        decimal_value = int(measured_value[::-1], 2)
        measured_values.append(decimal_value)
        print(f"Binary: {measured_value} -> Decimal: {decimal_value}, Count: {counts[measured_value]}")
    
    print("\nAnalyzing for factors:")
    print("-" * 30)
    
    factors_found = set()
    
    for i in counts:
        measured_value = int(i[::-1], 2)
        print(f"Analyzing measured value: {measured_value}")
        
        if measured_value % 2 != 0:
            print("  -> Measured value is odd, skipping")
            continue
        
        x = int((a ** (measured_value/2)) % 15)
        print(f"  -> a^(r/2) mod 15 = {a}^({measured_value}/2) mod 15 = {x}")
        
        if (x + 1) % 15 == 0:
            print("  -> x+1 = 0 mod 15, skipping")
            continue
        
        factor1 = gcd(x + 1, 15)
        factor2 = gcd(x - 1, 15)
        factors = (factor1, factor2)
        
        print(f"  -> gcd({x}+1, 15) = gcd({x+1}, 15) = {factor1}")
        print(f"  -> gcd({x}-1, 15) = gcd({x-1}, 15) = {factor2}")
        print(f"  -> Factors found: {factors}")
        
        if factor1 > 1 and factor1 < 15:
            factors_found.add(factor1)
        if factor2 > 1 and factor2 < 15:
            factors_found.add(factor2)
    
    print("\nSummary:")
    print(f"Non-trivial factors found: {sorted(factors_found)}")
    if factors_found:
        print(f"15 = {' × '.join(map(str, sorted(factors_found)))}")
    
    return factors_found

def main():
    """Main function to run the complete Shor's algorithm demonstration"""
    try:
        print("=" * 60)
        print("SHOR'S ALGORITHM - FACTORING 15")
        print("=" * 60)
        
        # Parameters
        n = 4  # Number of counting qubits
        m = 4  # Number of auxiliary qubits
        a = 7  # Base for modular exponentiation
        shots = 1000  # Number of measurements
        
        # Run Shor's algorithm
        counts, circuit = run_shors_algorithm(n, m, a, shots)
        
        # Analyze results
        factors = analyze_results(counts, a)
        
        print("\n" + "=" * 60)
        print("ALGORITHM COMPLETE")
        print("=" * 60)
        
        if factors:
            print(f"Successfully factored 15 = {' × '.join(map(str, sorted(factors)))}")
        else:
            print("No non-trivial factors found in this run. Try running again.")
        
    except Exception as e:
        print(f"Error occurred: {e}")
        print("Make sure you have qiskit and qiskit-aer installed:")
        print("pip install qiskit==0.46.1 qiskit-aer==0.13")

if __name__ == "__main__":
    main()
