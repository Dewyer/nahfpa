---
to: <%= name %>/<%= name %>.c
---
#include "<%= name %>.h"

<%= name %>* <%= name %>_new()
{
    // TODO Implement
    <%= name %>* self = (<%= name %>*) malloc(sizeof(self));
    return self;
}

void <%= name %>_free(<%= name %>* self)
{
	free(self);
}
