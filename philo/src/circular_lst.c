/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 10:30:41 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/18 12:13:12 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_list	*lst_new(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

size_t	lst_size(t_list *lst)
{
	size_t	i;
	t_list	*node;

	i = 0;
	node = lst;
	if (lst)
	{
		while (node != lst || !i)
		{
			node = node->next;
			i++;
		}
	}
	return (i);
}

void	lst_add_back(t_list **alst, t_list *new_node)
{
	if (alst && new_node)
	{
		if (!*alst)
		{
			*alst = new_node;
			new_node->prev = new_node;
			new_node->next = new_node;
		}
		else
		{
			(*alst)->prev->next = new_node;
			new_node->prev = (*alst)->prev;
			(*alst)->prev = new_node;
			new_node->next = *alst;
		}
	}
}

void	lst_clear(t_list *lst)
{
	t_list	*swap;
	size_t	i;
	size_t	len;

	len = lst_size(lst);
	i = 0;
	while (i < len)
	{
		swap = lst->next;
		free(lst->content);
		free(lst);
		lst = swap;
		i++;
	}
}
