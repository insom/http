.PHONY: all push docker
http:
	c++ -static -pthread http.cc -o http
all: docker push deploy
docker:
	docker build . -t http:latest
	docker tag http:latest ghcr.io/insom/http:latest
	docker tag http:latest gcr.io/insom-161401/http:latest
push:
	docker push ghcr.io/insom/http:latest
	docker push gcr.io/insom-161401/http:latest
deploy:
	gcloud run deploy --image gcr.io/insom-161401/http:latest
