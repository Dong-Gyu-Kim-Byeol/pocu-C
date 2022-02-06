a.exe abc def < inputs/input1.txt > outputs/output1.txt
a.exe abc defghi < inputs/input2.txt > outputs/output2.txt
a.exe abcdefg hijk < inputs/input3.txt > outputs/output3.txt
a.exe aba cde < inputs/input4.txt > outputs/output4.txt
a.exe -i abc cde < inputs/input5.txt > outputs/output5.txt
a.exe ab\n\t cdnt < inputs/input6.txt > outputs/output6.txt
a.exe a-b-\\-a cdefj < inputs/input7.txt > outputs/output7.txt
a.exe "--/\n\\""" cdens < inputs/input8.txt > outputs/output8.txt

a.exe a-c f-h < inputs/input9.txt > outputs/output9.txt
a.exe a-c-h A-CGH < inputs/input10.txt > outputs/output10.txt
a.exe -b-g AB-G < inputs/input11.txt > outputs/output11.txt
a.exe abc- AB-D < inputs/input12.txt > outputs/output12.txt

pause