--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   15:18:56 11/17/2021
-- Design Name:   
-- Module Name:   C:/Users/cmarc/Desktop/UNIVERSIDAD/4/PRIMER CUATRI/Practica4Pt2/test.vhd
-- Project Name:  Practica4Pt2
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: practica4_parte2
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test IS
END test;
 
ARCHITECTURE behavior OF test IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT practica4_parte2
    PORT(
         clock : IN  std_logic;
         reset : IN  std_logic;
         D : IN  std_logic_vector(3 downto 0);
         C : IN  std_logic_vector(3 downto 0);
         S : OUT  std_logic_vector(7 downto 0);
         AN : OUT  std_logic_vector(3 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal clock : std_logic := '0';
   signal reset : std_logic := '0';
   signal D : std_logic_vector(3 downto 0) := (others => '0');
   signal C : std_logic_vector(3 downto 0) := (others => '0');

 	--Outputs
   signal S : std_logic_vector(7 downto 0);
   signal AN : std_logic_vector(3 downto 0);

   -- Clock period definitions
   constant clock_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: practica4_parte2 PORT MAP (
          clock => clock,
          reset => reset,
          D => D,
          C => C,
          S => S,
          AN => AN
        );

   -- Clock process definitions
   clock_process :process
   begin
		clock <= '0';
		wait for clock_period/2;
		clock <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
		reset<='1';
      wait for 100 ns;	
	reset<='0';
      wait for 2 ms;	
      c<="0001";
      d<="0001";
      wait for 1 ms;	
      c<="0010";
      d<="0011";
      wait for 1 ms;	
      c<="0100";
      d<="0111";
      wait for 3 ms;	
      c<="1000";
      d<="0101";
      wait for 3 ms;	
      c<="0000";
      d<="0101";

      wait for clock_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
