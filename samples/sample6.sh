cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
(x+a)^cos(a-x)
0-i*110
0
i*100
400
300
-5.2
4.8
-4.8
7.2
1.1
10
100
EOF
