cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
(x+a)^sin(a-x)*tan(ln(x)+a)
0-i*200
0-i*50
i*255
400
300
2.2
3.0
1.9
2.0
1.1
10
100
EOF
