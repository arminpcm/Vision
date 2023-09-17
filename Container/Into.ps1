# Specify the name or ID of the Docker container you want to enter
$containerNameOrID = "vision"

# Execute a shell inside the running container (e.g., bash)
docker exec -it $containerNameOrID env TERM=xterm-256color /bin/bash