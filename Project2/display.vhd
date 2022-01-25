----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date:    15:19:28 10/06/2021
-- Design Name:
-- Module Name:    display_numbers - Behavioral
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

entity display_numbers is
    Port ( D : in  STD_LOGIC_VECTOR (3 downto 0);
           C : in  STD_LOGIC_VECTOR (3 downto 0);
           S : out  STD_LOGIC_VECTOR (7 downto 0);
           AN : out  STD_LOGIC_VECTOR (3 downto 0));
end display_numbers;

architecture Behavioral of display_numbers is
begin
process(C)
	begin
		case C is
			when "0001" =>
				AN<="1110";
			when "0010"=>
				AN<="1101";
			when "0100" =>
				AN<="1011";
			when "1000"=>
				AN<="0111";
			when others=>
				AN<= "1111";
		end case;
	end process;

	
process(D)
	begin
		case D is
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
