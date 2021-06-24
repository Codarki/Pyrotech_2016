//.-----------------.
//| P y r o t e c h |-----------------------------------------------------------
//'-----------------'
//| 
//| Author:   Lasse Lopperi
//| Filename: sorter.inl
//| Created:  6. joulukuuta 2004
//| 
//'---[ Versions ]--------------------------------------------------------------
//| 
//| v1.0 [6. joulukuuta 2004] : File created.
//| 
//'---------------------------------------------[ Copyright © Pyrotech, 2004 ]--

template<typename T>
void insertion_sort(T * input, int size, bool sort_ascending)
{
	T temp;

	if(sort_ascending)
	{
		for(size_t i=0; i<size; i++)
		{
			for(size_t j=i; j>0; j--)
			{
				if(input[j]<input[j-1])
				{
					temp = input[j];
					input[j] = input[j-1];
					input[j-1] = temp;
				} else break;
			}
		}
	}
	else
	{
		for(size_t i=0; i<size; i++)
		{
			for(size_t j=i; j>0; j--)
			{
				if(input[j]>input[j-1])
				{
					temp = input[j];
					input[j] = input[j-1];
					input[j-1] = temp;
				} else break;
			}
		}
	}
	return;
}
