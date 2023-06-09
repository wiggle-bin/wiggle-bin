<script lang="ts">
	import type { PlotType } from 'plotly.js-dist-min';
	import { start, end } from '../../store/store';
	import { onMount } from 'svelte';

    export let type: PlotType | undefined = 'bar';

	let startTime = '';
	let endTime = '';
	let plotContainer: HTMLElement;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));

	let plotly: null | any;
	$: plotly = null;
	$: contoursPlot(plotly, startTime, endTime);

	onMount(async () => {
		plotly = await import('plotly.js-dist-min');
	});

	async function contoursPlot(plotly: any, startTime: string, endTime: string) {
		if (!plotly) return;

		const response = await fetch(
			`http://127.0.0.1:8000/contours?startTime=${startTime}&endTime=${endTime}`
		);
		const data = await response.json();
        const dates = data.dates.map((date: string, index: number) => {
                const [year, month, dayAndTime] = date.split('-');
                const [day, time] = dayAndTime.split('_');
                const hour = time.slice(0, 2)
                const minute = time.slice(2, 4)
                return new Date(+year, +month, +day, +hour, +minute).getMinutes()
            })

		const trace1: Plotly.Data = {
			x: dates,
			y: data.contourArea,
			// line: { color: 'white' },
            // fillcolor: 'white',
			type,
            marker: {
                // color: 'rgba(255, 102, 107, 1)'
            }
		};

        console.log(data.contourArea)

        const trace2: Plotly.Data = {
			x: dates,
			y: [344.5, 167.5, 259, 229.5, 168.5, 448, 305.5, 162.5, 244.5, 182.5, 99.5, 124.5, 79.5, 74, 273.5, 129, 297.5],
			type
		};

        const trace3: Plotly.Data = {
			x: dates,
			y: [344.5, 167.5, 259, 124.5, 79.5, 74, 229.5, 168.5,  99.5, 448, 305.5, 162.5, 244.5, 182.5, 273.5, 129, 297.5],
			type
		};

		const plotlyData: Plotly.Data[] = [trace1, trace2, trace3];

		plotly.newPlot(plotContainer, plotlyData, {
			font: {
                family: '"open-sans", sans-serif',
                size: 12,
                color: 'black'
			},
			plot_bgcolor: 'white',
			paper_bgcolor: 'white',
            xaxis: {
                title: '<b>Time in minutes</b>',
                color: 'black'
            },
            yaxis: {
                title: '<b>Area of movement</b>',
                color: 'black'
            },
            title: {
                text: '<b>Contour area</b>'
            }
		}, {
            displayModeBar: false,
            displaylogo: false,
            responsive: true
        });
	}
</script>

<div bind:this={plotContainer}><!-- Plotly chart will be drawn inside this DIV --></div>