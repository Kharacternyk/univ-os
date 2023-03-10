FROM rocker/r-rmd
WORKDIR /R
COPY . .
RUN apt install r-cran-readr
RUN Rscript -e 'rmarkdown::render("lab-1.Rmd")'
