#!/bin/bash
PORT=${1:-8080}
DIR="$(cd "$(dirname "$0")" && pwd)/www"

echo "Serving $DIR on http://127.0.0.1:$PORT"
python3 -m http.server "$PORT" --directory "$DIR"
