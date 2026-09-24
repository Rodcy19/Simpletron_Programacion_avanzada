   # Simpletron_Programacion_avanzada
   Simulador Simpletron
   
   El simulador Simpletron interpreta y ejecuta programas escritos en SML (Simpletron Machine Language). Basado en Deitel H.M., Deitel P.J. (2004). *Cómo programar en C*.
   Requisitos:
   Un compilador de C (GCC, Clang o cualquier otro de su preferencia).
   Sistema operativo Windows, Linux o macOS.
   Con Code::Blocks / Dev-C++
   Cree un nuevo proyecto de "Consola" en C.
   Añada el archivo `simpletron_v2.c` al proyecto.
   Compile el proyecto utilizando la opción de compilación (F9 o los iconos correspondientes).
   Uso
   Una vez iniciado el programa, recibirá un mensaje de bienvenida y comenzará a recibir instrucciones con el siguiente formato:
      00 ?
   Ingrese sus instrucciones SML o palabras de datos, una línea por cada posición de memoria (00, 01, 02, ...).
   Para finalizar la entrada de datos del programa, ingrese `9999`.
   El simulador comenzará a ejecutar el programa. Si el programa contiene una instrucción READ (código 10), le solicitará que ingrese un número entero.
   Al finalizar el programa (ya sea por la instrucción HALT/43 o por un error fatal), se mostrará:
   Un mensaje de finalización (normal o anormal).
   El contenido de los registros (acumulador, contador de instrucciones, registro de instrucción, código de operación, operando).
   El volcado completo de la memoria (100 posiciones en una tabla de 10x10).
