FROM rocker/r-rmd
WORKDIR /R
COPY . .
RUN Rscript -e 'rmarkdown::render("lab-1.Rmd")'
