----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:56:03 10/13/2021 
-- Design Name: 
-- Module Name:    ultrasonido - Behavioral 
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ultrasonido is
    Port ( echo : in  STD_LOGIC;
           clock : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           trigger : out  STD_LOGIC;
           L : out  STD_LOGIC_VECTOR (3 downto 0));
end ultrasonido;


--500 ciclos tiene que estar a 1 el trigger
--299.500 ciclos tiene que estar a 0 el trigger
--total, 300000 ciclos


architecture Behavioral of ultrasonido is

	signal ncycle:INTEGER := 0;
	signal cnt: INTEGER:= 0;
	signal led:STD_LOGIC_VECTOR(3 downto 0);
	signal LH:STD_LOGIC := '0';
	
begin

	process(clock, reset)
	variable T_1:STD_LOGIC :='0';	
	begin
		if (reset = '1') then 
			cnt<= 0;
			led<="0000";
			--Apagar luces???;
		elsif clock'event and clock = '1' then
		   if(LH = '1') then
			   L<=led;
			   LH<='0';
		   end if;
			if (cnt= 0) then 
				trigger<='1';
				LH<= (not T_1) and '1';
				T_1:='1';
			--	cnt<=cnt+1; 
			elsif (cnt = 500) then
				trigger<='0';
				T_1:='0';
			--	cnt<= cnt+1;
			end if;
			if (cnt = 3000000) then
				if (ncycle >174000) then
						led<="1111";
				else 
						if (ncycle >130500) then
								led<="0111";
						else 
								if (ncycle>87000) then
										led<="0011";
								else 
										if (ncycle>43500) then	
												led<="0001";
										else
												led<="0000";
										end if;
								end if;
						end if;
				 end if;	
					cnt <=0; --Cuando vale 3 millones lo reiniciamos
			else 
					cnt<=cnt+1; --entramos aquí siempre excepto cuando vale 3 millones
			end if;
		end if;
end process;


process(echo, ncycle, clock, reset)
begin
	
	if (reset = '1') then 
		ncycle <=0; 

	elsif  (clock'event and clock = '1') then
	
		if(LH = '1') then
			ncycle<=0;
		end if;
		
		if(echo = '1') then
			ncycle<= ncycle+1;
		end if;
	end if;
end process;
	
end Behavioral;



