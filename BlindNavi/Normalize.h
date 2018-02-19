#pragma once

unsigned int normalizeDanger(double inputValue)
{
	if (inputValue == 0) { return 0; }
	{
		if (inputValue <= 600) { return 10; }
		else
		{
			if (inputValue <= 1000) { return 9; }
			else
			{
				if (inputValue <= 1500) { return 8; }
				else
				{
					if (inputValue <= 2000) { return 7; }
					else
					{
						if (inputValue <= 2500) { return 6; }
						else
						{
							if (inputValue <= 3000) { return 5; }
							else
							{
								if (inputValue <= 3500) { return 4; }
								else
								{
									if (inputValue <= 4000) { return 3; }
									else
									{
										if (inputValue <= 4500) { return 2; }
										else
										{
											if (inputValue <= 5000) { return 1; }
											else { return 0; }
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}