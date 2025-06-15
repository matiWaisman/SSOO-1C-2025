import math 

def funcion1():
    print("Hola soy la funcion 1")

def funcion2():
    print("Hola soy la funcion 2")

def perimetro() -> float: 
    return 2*math.pi

def es_multiplo_de(n:int, m:int) -> bool: 
    res: bool = (n%m)==0
    return res

def es_nombre_largo(nombre: str) -> bool: 
    longitud_nombre_mayor_o_igual_a_3 : bool = len(nombre) >= 3
    longitud_nombre_menor_o_igual_a_8 : bool = len(nombre) <= 8
    res : bool = longitud_nombre_mayor_o_igual_a_3 and longitud_nombre_menor_o_igual_a_8
    return res

print(es_nombre_largo("MatiasEliel"))