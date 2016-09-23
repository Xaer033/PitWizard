// Ghostware Games inc. 2016  -Julian Williams
#pragma once


namespace GG
{
	class RenderFactory;

	class RenderableObject 
	{
	public:
		virtual void	render( RenderFactory & renderFactory ) = 0;
	};
}