/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlaunch <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 14:52:29 by hlaunch           #+#    #+#             */
/*   Updated: 2021/10/15 14:52:30 by hlaunch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c > 47 && c < 58) || (c < 91 && c > 64) || (c > 96 && c < 123))
		return (1);
	return (0);
}
