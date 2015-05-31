##############################################
# runbench.R
#
# Archivo para la ejecución del benchmark, la
# recogida de resultados y la visualización y
# obtención de tests estadísticos asociados
##############################################

# Declaración de nombres
filenames <- c("results1.csv", "results2.csv", "results3.csv")
govnames <- c("performance", "ondemand", "conservative", "powersave")
tests <- c("INT1", "FLOAT1", "INT10", "FLOAT10", "INT100", "FLOAT100")

if (!all(file.exists(filenames))) {
  # Compilación del programa
  system("make")
  
  # Ejecución del benchmark para cada archivo de resultados
  sapply(filenames, function(fn) {
    system(paste("./govbench > ", fn, sep=""))
  })
}

# Recolección de resultados de los archivos CSV
results <- lapply(filenames, function(fn) 
  t(read.csv(fn, header = FALSE, row.names = govnames, col.names = c(tests, ""))[,1:length(tests)])
)


# Promedio de resultados a lo largo de las repeticiones
means <- data.frame(sapply(1:length(govnames), function(i)
  rowMeans(data.frame(r1=results[[1]][,i], r2=results[[2]][,i], r3=results[[3]][,i]))
))

means2 <- data.frame(time = unlist(means, use.names = FALSE),
                     test = tests,
                     governor = rep(govnames, each = length(tests)))

# Gráfico. Primero instalamos el paquete si procede
if (!require(ggplot2)) {
  install.packages("ggplot2")
  require(ggplot2)
}

# Guardar el gráfico en una imagen
png(filename = "results.png",
    units = "px",
    width = 800,
    height = 600,
    pointsize = 6,
    res = 90)

# Se genera el gráfico
pl <- ggplot(means2, aes(x = test, y = time, colour = governor, group = governor))
pl + geom_line()

dev.off()

# Promedio y reorganización de los datos en 2 columnas (para
# aplicar test ANOVA)
twocol <- data.frame(governor = govnames,
                     time = c(as.numeric(colMeans(results[[1]])),
                              as.numeric(colMeans(results[[2]])),
                              as.numeric(colMeans(results[[3]])))
                     )

# Aplicar test de análisis de la varianza (función aov)
gov.aov <- aov(time ~ governor, data = twocol, contrasts = c("performance", "powersave"))

# Imprimir resultados y comparación por parejas
# Fuente: http://www.ats.ucla.edu/stat/r/faq/posthoc.htm
writeLines("==== RESULTADOS DEL TEST ANOVA ====")
print(summary(gov.aov))
writeLines("\n==== COMPARACIÓN POR PAREJAS ====")
print(pairwise.t.test(twocol$time, twocol$governor, p.adj = "none"))
print(TukeyHSD(gov.aov))