// Ghostware Games inc. 2016  -Julian Williams
#pragma once


namespace GG
{
	class RenderFactory;

	class IRenderableObject 
	{
	public:
		virtual void	render( RenderFactory & renderFactory ) = 0;
	};
}