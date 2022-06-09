FROM docker.io/ethcomsec/cellift:cellift-tools-main
COPY . /cellift-designs/cellift-cva6
WORKDIR /cellift-designs/cellift-cva6/cellift
CMD bash tests.sh
