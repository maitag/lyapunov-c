cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
2.5*sin(x+a)^2-cos(a-x)^2
i*210
0-i*20
0-i*20
400
300
-2.3
-0.7
0.7
2.3
1.1
10
100
EOF
