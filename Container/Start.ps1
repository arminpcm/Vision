# Specify the name or ID of the Docker container you want to start
$containerNameOrID = "vision"

# Start the Docker container
docker run --privileged --memory="5g" --memory-swap="10g" --hostname vision -v C:/Users/17345/Documents/projects/Vision:/Vision --name $containerNameOrID -p 6379:6379 -itd $containerNameOrID
