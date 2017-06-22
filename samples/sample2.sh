cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
3.3*sin(x+a*ln(x))^2*cos(x/(a-x))^3
(3-i)*111
255-(1+i)*111
(1+i)*125
400
300
-4
1
-4
5
2.13455
10
100
EOF
