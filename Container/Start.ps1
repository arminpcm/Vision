# Specify the name or ID of the Docker container you want to start
$containerNameOrID = "vision"

# Start the Docker container
docker run --privileged -p 5173:5173 -p 6379:6379 --memory="5g" --memory-swap="10g" --hostname vision -v C:/Users/17345/Documents/projects/Vision:/Vision --name $containerNameOrID -itd $containerNameOrID
