
#include <Arduino.h>

#include <FreeTypeBuffer.h>
#include <FreeTypeRaster.h>

#include "Outlines/MaterialIcons-Regular.h"
#include "Outlines/Roboto-Light.h"

#define BAND_COUNT 7
#define BAND_HEIGHT 5
#define BAND_WIDTH 110

uint8_t buf[BAND_HEIGHT * BAND_WIDTH];
FreeTypeBuffer target(buf, BAND_HEIGHT *BAND_WIDTH);

FreeTypeRaster raster;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    for (int band = 0; band != BAND_COUNT; ++band)
    {
        target.reset(0, band * BAND_HEIGHT, BAND_WIDTH, BAND_HEIGHT);

        target.cursor(0, 30);
        raster.render(roboto::light::u0034, &target);

        target.cursor(40, 30);
        raster.render(roboto::light::u0030, &target);

        target.cursor(65, 34);
        raster.render(materialicons::regular::uE834, &target);

        uint8_t *scan = buf;
        for (int row = 0; row != BAND_HEIGHT; ++row)
        {
            for (int col = 0; col != BAND_WIDTH; ++col)
            {
                int opacity = *scan++;
                Serial.print(" .:-=+*#%@"[opacity * 10 / 256]);
            }
            Serial.println();
        }
    }

    delay(15000);
}

//                                                                        :=++++++++++++++++++++=:
//                                                                       *@@@@@@@@@@@@@@@@@@@@@@@@*
//             :@@-                              :*%@@%+:               :@@@@@@@@@@@@@@@@@@@@@@@@@@:
//             %@@-                             *@@@%%@@@+              =@@@@@@@@@@@@@@@@@@@@@@@@@@=
//            +@@@-                            *@@-    -@@+             +@@@@@@@@@@@@@@@@@@@@@@@@@@+
//           :@%#@-                           -@@.      .@@:            +@@@@@@@@@@@@@@@@@@@@@@@@@@+
//           %@:#@-                           %@-        =@#            +@@@@@@@@@@@@@@@@@@@@*.%@@@+
//          =@* #@-                          .@@          @@.           +@@@@@@@@@@@@@@@@@@@*  .%@@+
//         .@@  #@-                          =@*          #@-           +@@@@@@@@@@@@@@@@@@*   .%@@+
//         #@-  #@-                          *@=          +@+           +@@@@@@@@@@@@@@@@@*   .%@@@+
//        -@#   #@-                          #@-          -@#           +@@@@@@@@@@@@@@@@*   .%@@@@+
//        @@.   #@-                          %@:          -@#           +@@@@@@@@@@@@@@@*   .%@@@@@+
//       *@=    #@-                          %@:          :@%           +@@@%.#@@@@@@@@*   .%@@@@@@+
//      -@%     #@-                          %@:          :@%           +@@%.  #@@@@@@*   .%@@@@@@@+
//      %@:     #@-                          %@:          :@%           +@@%.   #@@@@*   .%@@@@@@@@+
//     +@+      #@-                          %@:          :@%           +@@@%.   #@@*   .%@@@@@@@@@+
//    :@%       #@-                          %@:          :@%           +@@@@%.   #*   .%@@@@@@@@@@+
//    %@-       #@-                          %@:          :@%           +@@@@@%.      .%@@@@@@@@@@@+
//   +@#        #@-                          %@:          -@#           +@@@@@@%.    .%@@@@@@@@@@@@+
//  .@@.        #@-                          #@-          -@#           +@@@@@@@%.  .%@@@@@@@@@@@@@+
//  #@@@@@@@@@@@@@@@@@-                      *@=          +@+           +@@@@@@@@%..%@@@@@@@@@@@@@@+
//  *###########@@%###:                      =@*          #@-           +@@@@@@@@@%%@@@@@@@@@@@@@@@+
//              #@-                          .@@          @@.           +@@@@@@@@@@@@@@@@@@@@@@@@@@+
//              #@-                           %@=        =@#            +@@@@@@@@@@@@@@@@@@@@@@@@@@+
//              #@-                           :@@.      .@@:            =@@@@@@@@@@@@@@@@@@@@@@@@@@=
//              #@-                            *@@-    -@@*             :@@@@@@@@@@@@@@@@@@@@@@@@@@:
//              #@-                             +@@@%%@@@+               *@@@@@@@@@@@@@@@@@@@@@@@@*
//              #@-                              :*%@@%*:                 :=++++++++++++++++++++=:
