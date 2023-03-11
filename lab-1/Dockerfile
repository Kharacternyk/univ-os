FROM nixos/nix

RUN nix-channel --update
RUN nix-env -iA nixpkgs.sl
RUN sl

#FROM rocker/r-rmd
#WORKDIR /R
#COPY . .
#RUN apt install --yes r-cran-readr
#RUN Rscript -e 'rmarkdown::render("lab-1.Rmd")'
