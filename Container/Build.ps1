# Define the path to the Dockerfile and the desired image tag
$dockerfilePath = "C:\Users\17345\Documents\projects\vision\container\Dev.dockerfile"
$imageTag = "vision"

# Build the Docker image using the specified Dockerfile
docker build -f $dockerfilePath -t $imageTag .

# Check if the build was successful
if ($LASTEXITCODE -eq 0) {
    Write-Host "Docker image '$imageTag' was successfully built."
} else {
    Write-Host "Docker image build failed. Check the Dockerfile and try again."
}
