#pragma once


class DiagnosticTools
{
protected:
	Logger log;			// Logger of the custom allocator
	DiagnoseExternalFragmentation diagExternal;  // External diagnostic of the custom allocator
	Diagnostics diag;	// Diagnostics of the custom allocator
};