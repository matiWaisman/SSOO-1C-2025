echo -n "Ingresa el numero a: "
read inputa
a=inputa
echo -n "Ingresa el numero b: "
read inputb
b=inputb
suma=$(($a + $b))
producto=$(($a * $b))
if [ $suma>=$producto ]
then 
    echo "La suma es mayor o igual al producto"
fi
if [ $suma<=$producto ]
then 
    echo "La suma es menor o igual al producto"
fi
if [ $suma==$producto ]
then 
    echo "La suma es igual al producto"
fi

