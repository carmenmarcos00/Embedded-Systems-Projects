----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date:    15:19:28 10/06/2021
-- Design Name:
-- Module Name:    practica4parte2 - Behavioral
-- Project Name:
-- Target Devices:
-- Tool versions:
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity practica4_parte2 is
    Port ( clock : in STD_LOGIC; 
			  reset : in STD_LOGIC; 
			  D : in  STD_LOGIC_VECTOR (3 downto 0);
           C : in  STD_LOGIC_VECTOR (3 downto 0);
           S : out  STD_LOGIC_VECTOR (7 downto 0);
           AN : out  STD_LOGIC_VECTOR (3 downto 0));
end practica4_parte2;

architecture Behavioral of practica4_parte2 is

signal contador: INTEGER; 
signal r0: STD_LOGIC_VECTOR (3 downto 0);
signal r1: STD_LOGIC_VECTOR (3 downto 0);
signal r2: STD_LOGIC_VECTOR (3 downto 0);
signal r3: STD_LOGIC_VECTOR (3 downto 0);
signal R: STD_LOGIC_VECTOR (3 downto 0);

signal state: STD_LOGIC_VECTOR(1 downto 0);

begin 
process(clock, reset)	
	begin
	
	if (reset = '1') then 
		--cuando reseteamos todos los registros a 0 para que se apage todo
		r0<="1111";
		r1<="1111";--configuracion de interruptores, apaga display, nice
		r2<="1111";
		r3<="1111";
	else if  (clock'event and clock = '1') then
			case C is
				when "0001" =>
					r3<=D;
				when "0010" =>
					r2<=D;
				when "0100" =>
					r1<=D;
				when "1000" =>
					r0<=D;
				when others=>
				--si se pulsan 2 a la vez no ocurre nada
			end case;
		end if; 
	end if; 
end process;

process (state, r0, r1, r2, r3) --maquina de estados
	begin --cada 50000 cambio de estado
			--Cuando pasen 5000 cycles cambiamos de estado
			case state is
				when "00" => --que ocurre en cada estado
					--mandar r0 a s
					R<=r0;
					AN<="0111";
				when "01" => 
					R<=r1;
					AN<="1011";
				when "10" =>
					R<=r2;
					AN<="1101";
				when "11" =>
					R<=r3;
					AN<="1110";
				when others =>	
					--No hacemos nada
			end case;
end process;

process (clock, reset) --maquina de estados
begin

	if (reset = '1') then 
		contador <=0;
		state<="00";
		else if  (clock'event and clock = '1') then --cambiar por elsif
			if (contador = 50000) then
				case State is
					when "00" =>
						state<= "01";
					when "01" =>
						state<= "10";
					when "10" =>
						state<= "11";
					when "11" =>
						state<= "00";
					when others =>	
				end case;
				contador <=0;
				else 
					contador <= contador +1;
				end if; 
			end if; 
		end if; 
end process;

process (R)
begin
	case R is 
		when "0000" =>
			S<="00000011";
		when "0001"=>
			S<="10011111";
		when "0010" =>
			S<="00100101";
		when "0011"=>
			S<="00001101";
		when "0100" =>
			S<="10011001";
		when "0101"=>
			S<="01001001";
		when "0110" =>
			S<="01000001";
		when "0111"=>
			S<="00011111";
		when "1000" =>
			S<="00000001";
		when "1001"=>
			S<="00001001";
		when others =>
			S<="11111111";	
	end case;	
end process;

end Behavioral;

