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
           S : out  STD_LOGIC_VECTOR (7 downto 0);
           AN : out  STD_LOGIC_VECTOR (3 downto 0));
end ultrasonido;


--500 ciclos tiene que estar a 1 el trigger
--299.500 ciclos tiene que estar a 0 el trigger
--total, 300000 ciclos


architecture Behavioral of ultrasonido iS
	signal ncycle:INTEGER := 0;
	signal cnt: INTEGER:= 0;
	signal LH:STD_LOGIC := '0';
	signal tmp:STD_LOGIC_VECTOR(3 downto 0);
	
	signal contador: INTEGER; 
	signal r0: STD_LOGIC_VECTOR (3 downto 0);
	signal r1: STD_LOGIC_VECTOR (3 downto 0);
	signal r2: STD_LOGIC_VECTOR (3 downto 0);
	signal r3: STD_LOGIC_VECTOR (3 downto 0);
	signal R: STD_LOGIC_VECTOR (3 downto 0);
	signal state: STD_LOGIC_VECTOR(1 downto 0);
	
begin
	process(clock, reset)
	variable T_1:STD_LOGIC :='0';	
	begin
		if (reset = '1') then 
			cnt<= 0;
			--Apagar luces???;
		elsif clock'event and clock = '1' then
		   if(LH = '1') then
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
						tmp<="0100";
				else 
						if (ncycle >130500) then
								tmp<="0011";
						else 
								if (ncycle>87000) then
										tmp<="0010";
								else 
										if (ncycle>43500) then	
												tmp<="0001";
										else
												tmp<="0000";
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

process(tmp)
begin
	
	case tmp is
				when "0000" => --que ocurre en cada estado
					--mandar r0 a s
					r0<="0000";
					r1<="0000";
					r2<="0000";
					r3<="0000";
				when "0001" => 
					r0<="0000";
					r1<="0000";
					r2<="0001";
					r3<="0101";
				when "0010" =>
					r0<="0000";
					r1<="0000";
					r2<="0011";
					r3<="0000";
				when "0011" =>
					r0<="0000";
					r1<="0000";
					r2<="0100";
					r3<="0101";
				when "0100" =>
					r0<="0000";
					r1<="0000";
					r2<="0110";
					r3<="0000";
				when others =>	
					--No hacemos nada
			end case;
	
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



