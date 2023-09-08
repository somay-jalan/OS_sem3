# !bin/bash
# x y operation


while read -r line 
do 
    read -r x y op <<< "$line"

    if [ "$op" == "compare" ]
    then
        #compare 
        if (($x > $y))
        then
            echo "Result of compare $x $y : $x"
        else
            echo "Result of compare $x $y : $y"
        fi
        
    elif [ "$op" == "xor" ]
    then
        #xor
        xor=$(( $x ^ $y ))
        echo "Result of xor $x $y : $xor"

    elif [ "$op" == "product" ]
    then
        #product
        mul=$(( $x * $y ))
        echo "Result of product $x $y : $mul"

    else
        echo "Error"
    fi

done < Q3IN.txt

mkdir -p Q3
./Q3.sh > Q3/Q3OUT.txt
