#!/bin/bash
set -x -e
result=$(mktemp --suffix=.result)
log=$(mktemp --suffix=.log)
script=$(mktemp --suffix=.sh)
trap 'rm -f $result $log $script' EXIT

ulimit 512

cat > "$script" <<EOF
#!/bin/bash
(set -x -e; ./xseturgent $*) 2>&1 | tee $log
echo \${PIPESTATUS[0]} > $result
EOF

xvfb-run -a xterm -e /bin/bash "$script"
cat "$log"
exit "$(cat "$result")"
