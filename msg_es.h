#ifndef __MSG
#define __MSG

#include <string>

using std::string;

namespace Message {
    const string WARNING = "==== AVISO ====",
                 ERROR   = "==== ERROR ====";
    const char CHAR_Y = 'y',
               CHAR_N = 'n';
    const string CONFIRM_Y = " (Y/n): ",
                 CONFIRM_N = " (y/N): ";
    
    const string
        NO_ROOT = ERROR + "\n" + 
" Esta aplicación debe ejecutarse como superusuario. Abortando.",
        PSTATE_ENABLED = WARNING + "\n" +
" El driver Intel P-state está activado. Esto reducirá el número\n\
 de pruebas que se puedan llevar a cabo y alterará notablemente\n\
 los resultados, puesto que los governors tendrán un comportamiento\n\
 distinto. ¿Desea continuar?",
        DISABLE_PSTATE = 
" Para desactivar Intel P-state, añada el parámetro \e[1mintel_pstate=disable\e[m\n\
 a la línea del kernel en el arranque.",
        ERROR_SET_GOVERNOR = ERROR + "\n" +
" Hubo un problema al modificar el 'scaling governor'."
    ;
    string USING_GOV(string gov) {
        return " \e[1mPasando a usar el governor " + gov + "\e[m";
    }
}

#endif