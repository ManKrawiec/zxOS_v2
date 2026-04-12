// PIC.cpp
// Written by TechEverything

/*
Handles remapping the Programmable Interrrupt Controller and EOI
*/

/*
0x20 - master command
0x21 - master data

0xA0 - slave command
0xA1 - slave data
*/

#include "Functions.hpp"
#include "Integers.hpp"

#include "zx/Ports/IO.hpp"

#include "zx/Interrupts/PIC.hpp"

namespace PIC {
    namespace Commands {
        namespace PIC {
            u16 EOI = 0x20;
        };

        namespace PIC1 {
            u16 Command = 0x20;
            u16 Data = 0x21;
        };

        namespace PIC2 {
            u16 Command = 0xA0;
            u16 Data = 0xA1;
        };

        namespace ICW1 {
            u16 ICW4 = 0x01;
            u16 Single = 0x02;
            u16 Interval = 0x04;
            u16 ICW1Level = 0x08;
            u16 Init = 0x10;
        };

        namespace ICW4 {
            u16 _8086 = 0x01;
            u16 Auto = 0x02;
            u16 BufferedSlave = 0x08;
            u16 BufferedMaster = 0x0C;
            u16 SFNM = 0x10;
        };

        u16 CascadeIRQ = 2;
    }

    function return_type(void) SendEOI(u8 irq) {
        if (irq >= 8)
            IO::Output::Byte(Commands::PIC2::Command, Commands::PIC::EOI);
        IO::Output::Byte(Commands::PIC1::Command, Commands::PIC::EOI);
    }

    function return_type(void) Remap(i32 offset_1, i32 offset_2) {
        IO::Output::Byte(
            Commands::PIC1::Command, 
            Commands::ICW1::Init | Commands::ICW1::ICW4
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC2::Command, 
            Commands::ICW1::Init | Commands::ICW1::ICW4
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC1::Data, 
            offset_1
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC2::Data, 
            offset_2
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC1::Data, 
            1 << Commands::CascadeIRQ
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC2::Data, 
            2
        );

	    IO::Methods::Hiccup();
        
	    IO::Output::Byte(
            Commands::PIC1::Data, 
            Commands::ICW4::_8086
        );

	    IO::Methods::Hiccup();

	    IO::Output::Byte(
            Commands::PIC2::Data, 
            Commands::ICW4::_8086
        );

	    IO::Methods::Hiccup();
	
	    IO::Output::Byte(
            Commands::PIC1::Data, 
            0
        );

	    IO::Output::Byte(
            Commands::PIC2::Data, 
            0
        );
    }
    
    namespace IRQ {
        function return_type(void) SetMask(u8 IRQline) {
            u16 port;
            u8 value;

            if (IRQline < 8) {
                port = Commands::PIC1::Data;
            } else {
                port = Commands::PIC2::Data;
                IRQline -= 8;
            }

            value = IO::Input::Byte(port) | (1 << IRQline);
            IO::Output::Byte(port, value);        
        }

        function return_type(void) ClearMask(u8 IRQline) {
            u16 port;
            u8 value;

            if (IRQline < 8) {
                port = Commands::PIC1::Data;
            } else {
                port = Commands::PIC2::Data;
                IRQline -= 8;
            }
            value = IO::Input::Byte(port) & ~(1 << IRQline);
            IO::Output::Byte(port, value);        
        }
    }
}