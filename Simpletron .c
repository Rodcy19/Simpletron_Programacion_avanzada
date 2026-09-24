#include <stdio.h>

#define CAPACIDAD 100
#define LIMITE_INF -9999
#define LIMITE_SUP 9998

/* --- Codigos de operacion SML --- */
#define OP_READ         10
#define OP_WRITE        11
#define OP_LOAD         20
#define OP_STORE        21
#define OP_ADD          30
#define OP_SUBTRACT     31
#define OP_DIVIDE       32
#define OP_MULTIPLY     33
#define OP_BRANCH       40
#define OP_BRANCHNEG    41
#define OP_BRANCHZERO   42
#define OP_HALT         43

/* Registros de la maquina, agrupados en una sola estructura */
typedef struct {
    int acc;   /* acumulador */
    int pc;    /* contador de instrucciones */
    int ir;    /* registro de instrucciones */
    int op;    /* codigo de operacion */
    int dir;   /* operando / direccion */
} Registros;

static int RAM[CAPACIDAD];

static void mostrarBienvenida(void);
static int  cargarPrograma(int RAM[]);
static int  correrCiclo(int RAM[], Registros *reg);
static void volcarMemoria(int RAM[], const Registros *reg);

int main(void)
{
    Registros reg = {0, 0, 0, 0, 0};
    int huboError;

    mostrarBienvenida();
    cargarPrograma(RAM);

    printf("\n***Se termino de cargar el programa***\n");
    printf("***Comienza la ejecucion del programa ***\n\n");

    huboError = correrCiclo(RAM, &reg);

    if (huboError)
    {
        printf("***La ejecucion de Simpletron termino anormalmente");
    }

    volcarMemoria(RAM, &reg);

    return 0;
}

/* Imprime los mensajes iniciales del simulador */
static void mostrarBienvenida(void)
{
    printf("***Bienvenidos a SIMPLETRON ***\n");
    printf("***Introduzca su programa una instruccion ***\n");
    printf("***(o palabra de datos)a la vez en la linea ***\n\n");
    printf("***de texto de entrada.Yo indicare el numero***\n");
    printf("***de posicion y una interrogacion(?).Usted ***\n");
    printf("***tecleara una palabra para esa posicion.Capture ***\n\n");
    printf("***el numero 9999 para terminar de introducir ***\n");
    printf("***su programa ***\n");
}

/* Fase de carga: solicita instrucciones/datos y las coloca en RAM */
static int cargarPrograma(int RAM[])
{
    int direccion = 0;
    int dato;

    for (direccion = 0; direccion < CAPACIDAD; )
    {
        printf("%02d ?", direccion);
        scanf("%d", &dato);

        if (dato == 9999)
        {
            break;
        }

        if (dato < LIMITE_INF || dato > LIMITE_SUP)
        {
            printf("***¡¡Valor fuera de Rango!!\n");
        }
        else
        {
            RAM[direccion] = dato;
            direccion++;
        }
    }

    return direccion;
}

/* Fase de ejecucion: ciclo fetch-decode-execute.
   Regresa 1 si hubo un error fatal, 0 si termino de forma normal. */
static int correrCiclo(int RAM[], Registros *reg)
{
    int activo = 1;
    int hizoBrinco;
    int dato;
    int error = 0;

    while (activo)
    {
        if (reg->pc < 0 || reg->pc >= CAPACIDAD)
        {
            printf("***Direccion de memoria no valida***\n");
            error = 1;
            activo = 0;
            break;
        }

        reg->ir = RAM[reg->pc];
        reg->op = reg->ir / 100;
        reg->dir = reg->ir % 100;
        hizoBrinco = 0;

        switch (reg->op)
        {
            case OP_READ:
                printf("Teclee un entero");
                scanf("%d", &dato);
                while (dato < LIMITE_INF || dato > LIMITE_SUP)
                {
                    printf("***Valor fuera de rango***\n");
                    printf("Teclee un Entero");
                    scanf("%d", &dato);
                }
                RAM[reg->dir] = dato;
                break;

            case OP_WRITE:
                printf("%02d => %d\n", reg->dir, RAM[reg->dir]);
                break;

            case OP_LOAD:
                reg->acc = RAM[reg->dir];
                break;

            case OP_STORE:
                RAM[reg->dir] = reg->acc;
                break;

            case OP_ADD:
                reg->acc = reg->acc + RAM[reg->dir];
                if (reg->acc > 9999 || reg->acc < -9999)
                {
                    printf("***Desbordamiento del acumulador***\n");
                    error = 1;
                    activo = 0;
                }
                break;

            case OP_SUBTRACT:
                reg->acc = reg->acc - RAM[reg->dir];
                if (reg->acc > 9999 || reg->acc < -9999)
                {
                    printf("***Desbordamiento del acumulado***\n");
                    error = 1;
                    activo = 0;
                }
                break;

            case OP_DIVIDE:
                if (RAM[reg->dir] == 0)
                {
                    printf("***No se puede dividir entre cero***\n");
                    error = 1;
                    activo = 0;
                }
                else
                {
                    reg->acc = reg->acc / RAM[reg->dir];
                }
                break;

            case OP_MULTIPLY:
                reg->acc = reg->acc * RAM[reg->dir];
                if (reg->acc > 9999 || reg->acc < -9999)
                {
                    printf("***Desbordamiento del acumulador***\n");
                    error = 1;
                    activo = 0;
                }
                break;

            case OP_BRANCH:
                reg->pc = reg->dir;
                hizoBrinco = 1;
                break;

            case OP_BRANCHNEG:
                if (reg->acc < 0)
                {
                    reg->pc = reg->dir;
                    hizoBrinco = 1;
                }
                break;

            case OP_BRANCHZERO:
                if (reg->acc == 0)
                {
                    reg->pc = reg->dir;
                    hizoBrinco = 1;
                }
                break;

            case OP_HALT:
                printf("***Termino la ejecucion de Simpletron***\n");
                activo = 0;
                break;

            default:
                printf("***Codigo de operacion no es valido***\n");
                error = 1;
                activo = 0;
                break;
        }

        if (activo && !hizoBrinco)
        {
            reg->pc++;
        }
    }

    return error;
}

/* Imprime el vaciado de registros y de la memoria completa */
static void volcarMemoria(int RAM[], const Registros *reg)
{
    int fila, col, indice;

    printf("\n***REGISTROS***:\n");
    printf("accumulator:                       %+05d\n", reg->acc);
    printf("instructionCounter:                %05d\n", reg->pc);
    printf("instructionRegister:               %+05d\n", reg->ir);
    printf("operationCode:                     %02d\n", reg->op);
    printf("operand:                           %02d\n", reg->dir);

    printf("\n***MEMORIA***\n\n");

    printf(" ");
    for (col = 0; col < 10; col++)
    {
        printf("%5d", col);
    }
    printf("\n");

    for (fila = 0; fila < 10; fila++)
    {
        printf("%d", fila);
        for (col = 0; col < 10; col++)
        {
            indice = fila * 10 + col;
            printf("%+05d", RAM[indice]);
        }
        printf("\n");
    }
}